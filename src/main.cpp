#include <Arduino.h>
#include <WiFi.h>
#include <lwip/sockets.h>
#include <errno.h>
#include "transport.h"

#ifndef A4_AP
#error Select the ap or transmitter PlatformIO environment.
#endif

constexpr char kSsid[] = "CS640-A4-10";
constexpr char kPassword[] = "cs640wifi";
constexpr char kRun[] = "q1-001";  // Change on both boards for each recorded run.
QueueHandle_t messages;
int rawSocket = -1;

#define LOG(...) do { Serial.printf("[%lu ms][%s] ", \
  static_cast<unsigned long>(millis()), kRun); Serial.printf(__VA_ARGS__); } while (0)

void fatal(const char *reason) {
  LOG("FATAL: %s errno=%d\n", reason, errno);
  for (;;) delay(1000);
}

#if A4_AP
void applicationTask(void *) {
  a4::Message message{};
  uint32_t expected = 1;
  uint32_t totalBytes = 0;
  bool allValid = true;
  for (;;) {
    LOG("APP WAIT: blocking on port 6401 queue\n");
    const uint32_t started = millis();
    // Blocks without polling when the eight-slot application queue is empty.
    if (xQueueReceive(messages, &message, portMAX_DELAY) != pdTRUE) continue;
    const bool valid = a4::verify(message, expected);
    allValid = allValid && valid;
    totalBytes += message.length;
    LOG("APP delivery number=%lu expected=%lu length=%u wait_ms=%lu Q=%u verify=%s\n",
        static_cast<unsigned long>(a4::read32(message.payload)),
        static_cast<unsigned long>(expected), message.length,
        static_cast<unsigned long>(millis() - started),
        static_cast<unsigned>(uxQueueMessagesWaiting(messages)), valid ? "PASS" : "FAIL");
    if (expected == a4::kMessages) {
      LOG("APP SUMMARY messages=20 bytes=%lu order/length/every-byte=%s\n",
          static_cast<unsigned long>(totalBytes), allValid ? "PASS" : "FAIL");
    }
    ++expected;
  }
}

void transportTask(void *) {
  uint8_t packet[1536];
  a4::Message message{};
  bool headerPrinted = false;
  for (;;) {
    const int size = recv(rawSocket, packet, sizeof(packet), 0);
    if (size < 0) { LOG("recv error=%d\n", errno); vTaskDelay(pdMS_TO_TICKS(10)); continue; }
    if (size < 20 || (packet[0] >> 4) != 4 || packet[9] != 253) continue;
    const size_t ihl = (packet[0] & 0x0f) * 4;
    if (ihl < 20 || ihl + a4::kHeader > static_cast<size_t>(size) ||
        a4::read16(packet + 2) != size || (a4::read16(packet + 6) & 0x3fff)) continue;
    uint8_t *header = packet + ihl;
    uint32_t received = 0, computed = 0;
    if (!a4::decode(header, size - ihl, message, received, computed)) {
      LOG("RX discard malformed/wrong-port/bad-CRC received=%08lx computed=%08lx\n",
          static_cast<unsigned long>(received), static_cast<unsigned long>(computed));
      continue;
    }
    if (!headerPrinted) {
      LOG("DATA HEADER hex:");
      for (size_t i = 0; i < a4::kHeader; ++i) Serial.printf(" %02x", header[i]);
      Serial.println();
      LOG("DECODE src=%u dst=%u seq=%lu ack=%lu length=%u window=%u CRC=%08lx recomputed=%08lx PASS IHL=%u\n",
          a4::read16(header), a4::read16(header + 2),
          static_cast<unsigned long>(a4::read32(header + 4)),
          static_cast<unsigned long>(a4::read32(header + 8)), a4::read16(header + 12),
          a4::read16(header + 14), static_cast<unsigned long>(received),
          static_cast<unsigned long>(computed), static_cast<unsigned>(ihl));
      LOG("Peer IP=%u.%u.%u.%u\n", packet[12], packet[13], packet[14], packet[15]);
      headerPrinted = true;
    }
    LOG("RX seq=%lu length=%u CRC=PASS\n",
        static_cast<unsigned long>(a4::read32(header + 4)), message.length);
    if (xQueueSend(messages, &message, 0) != pdTRUE) LOG("RX discard: queue full\n");
    // Step 1 deliberately sends no ACKs.
  }
}
#else
void applicationTask(void *) {
  a4::Message message{};
  for (uint32_t sequence = 1; sequence <= a4::kMessages; ++sequence) {
    a4::generate(message, sequence);
    xQueueSend(messages, &message, portMAX_DELAY);
  }
  vTaskDelete(nullptr);
}
void transportTask(void *) {
  sockaddr_in destination{};
  destination.sin_family = AF_INET;
  destination.sin_addr.s_addr = inet_addr("192.168.4.1");
  // The raw socket has no UDP/TCP port; ports are in our custom header.
  a4::Message message{};
  uint8_t packet[a4::kHeader + a4::kMaxPayload];
  LOG("IP acquired; AP receiver ready before association. First send delayed 2000 ms\n");
  vTaskDelay(pdMS_TO_TICKS(2000));
  for (uint32_t sequence = 1; sequence <= a4::kMessages; ++sequence) {
    xQueueReceive(messages, &message, portMAX_DELAY);
    const size_t length = a4::encode(packet, message);
    const int sent = sendto(rawSocket, packet, length, 0,
                            reinterpret_cast<sockaddr *>(&destination), sizeof(destination));
    LOG("TX first seq=%lu payload=%u transport_bytes=%u result=%d errno=%d\n",
        static_cast<unsigned long>(sequence), message.length,
        static_cast<unsigned>(length), sent, sent < 0 ? errno : 0);
    if (sequence < a4::kMessages) vTaskDelay(pdMS_TO_TICKS(100));
  }
  LOG("TX complete: 20 attempts; no ACK/retry in Step 1\n");
  vTaskDelete(nullptr);
}
#endif

void setup() {
  Serial.begin(115200);
  delay(1000);
  LOG("firmware=a4-q1-v1 role=%s N=20 slots=8 app_delay=0 send_interval=100 first_delay=2000 faults=none ACK/retry=disabled\n",
      A4_AP ? "AP" : "transmitter");
  const uint8_t crcTest[] = "123456789";
  if (a4::crc32(crcTest, 9) != 0xcbf43926) fatal("CRC self-test failed");
  LOG("CRC self-test 123456789 = CBF43926 PASS\n");
  messages = xQueueCreate(8, sizeof(a4::Message));
  if (!messages) fatal("queue allocation");
  // Initialize the IP stack without advertising an AP yet.
  WiFi.mode(WIFI_STA);
  rawSocket = socket(AF_INET, SOCK_RAW, 253);
  if (rawSocket < 0) fatal("raw protocol-253 socket");
#if A4_AP
  const IPAddress ip(192, 168, 4, 1), mask(255, 255, 255, 0);
  if (xTaskCreate(applicationTask, "application", 4096, nullptr, 1, nullptr) != pdPASS ||
      xTaskCreate(transportTask, "transport", 6144, nullptr, 2, nullptr) != pdPASS)
    fatal("task creation");
  // softAPConfig also enables the AP: receiver resources must exist first.
  if (!WiFi.softAPConfig(ip, ip, mask)) fatal("AP address configuration");
  if (!WiFi.softAP(kSsid, kPassword, 1, false, 1)) fatal("AP startup");
  WiFi.mode(WIFI_AP);
  LOG("AP READY SSID=%s IP=%s /24 DHCP enabled protocol=253 port=6401\n",
      kSsid, WiFi.softAPIP().toString().c_str());
#else
  WiFi.begin(kSsid, kPassword);
  while (WiFi.status() != WL_CONNECTED || WiFi.localIP() == IPAddress(0, 0, 0, 0)) delay(100);
  LOG("Client IP=%s AP IP=%s\n", WiFi.localIP().toString().c_str(), WiFi.gatewayIP().toString().c_str());
  if (xTaskCreate(applicationTask, "application", 4096, nullptr, 1, nullptr) != pdPASS ||
      xTaskCreate(transportTask, "transport", 6144, nullptr, 2, nullptr) != pdPASS)
    fatal("task creation");
#endif
}
void loop() { delay(1000); }
