#include <Arduino.h>
#include <WiFi.h>

// Lab group 10.
constexpr char kSsid[] = "CS640-A4-10";
constexpr char kPassword[] = "cs640lab4";
const IPAddress kApIp(192, 168, 4, 1);
const IPAddress kSubnet(255, 255, 255, 0);

void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
  if (event == ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED) {
    const IPAddress clientIp(info.wifi_ap_staipassigned.ip.addr);
    Serial.printf("[%lu ms] Client DHCP IP: %s\n", millis(),
                  clientIp.toString().c_str());
  } else if (event == ARDUINO_EVENT_WIFI_AP_STADISCONNECTED) {
    Serial.printf("[%lu ms] Client disconnected\n", millis());
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nLab 4 section 1.1 | firmware: ap-1.1 | role: AP");

  WiFi.onEvent(onWiFiEvent);
  // Arduino softAP starts the DHCP server automatically.
  if (!WiFi.mode(WIFI_AP) ||
      !WiFi.softAPConfig(kApIp, kApIp, kSubnet) ||
      !WiFi.softAP(kSsid, kPassword, 1, false, 1)) {
    Serial.println("ERROR: AP startup failed. Reset the board to retry.");
    WiFi.mode(WIFI_OFF);
    return;
  }

  Serial.printf("SSID: %s\n", kSsid);
  Serial.printf("AP IP: %s | subnet: %s\n",
                WiFi.softAPIP().toString().c_str(),
                kSubnet.toString().c_str());
  Serial.println("AP active; waiting for client DHCP assignment.");
  Serial.println("Transport receiver not implemented yet; do not send DATA.");
}

void loop() {
  delay(1000);
}
