#pragma once
#include <stddef.h>
#include <stdint.h>

namespace a4 {
constexpr size_t kHeader = 20;
constexpr size_t kMaxPayload = 1460;
constexpr uint32_t kMessages = 20;
struct Message {
  uint16_t length;
  uint8_t payload[kMaxPayload];
};
inline uint16_t read16(const uint8_t *p) {
  return (uint16_t(p[0]) << 8) | p[1];
}
inline uint32_t read32(const uint8_t *p) {
  return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) |
         (uint32_t(p[2]) << 8) | p[3];
}
inline void write16(uint8_t *p, uint16_t n) {
  p[0] = n >> 8; p[1] = n;
}
inline void write32(uint8_t *p, uint32_t n) {
  p[0] = n >> 24; p[1] = n >> 16; p[2] = n >> 8; p[3] = n;
}
inline uint32_t crc32(const uint8_t *p, size_t length) {
  uint32_t crc = 0xffffffff;
  for (size_t i = 0; i < length; ++i) {
    crc ^= p[i];
    for (int bit = 0; bit < 8; ++bit)
      crc = (crc >> 1) ^ ((crc & 1) ? 0xedb88320U : 0U);
  }
  return crc ^ 0xffffffff;
}
inline void generate(Message &m, uint32_t sequence) {
  m.length = sequence == kMessages ? 17 : kMaxPayload;
  write32(m.payload, sequence);
  for (size_t j = 4; j < m.length; ++j) m.payload[j] = (sequence + j) % 256;
}
inline size_t encode(uint8_t *packet, const Message &m) {
  write16(packet, 6400); write16(packet + 2, 6401);
  write32(packet + 4, read32(m.payload)); write32(packet + 8, 0);
  write16(packet + 12, m.length); write16(packet + 14, 0);
  write32(packet + 16, 0);
  for (size_t i = 0; i < m.length; ++i) packet[kHeader + i] = m.payload[i];
  const size_t length = kHeader + m.length;
  write32(packet + 16, crc32(packet, length));
  return length;
}
// Input begins at the custom header, after the IPv4 IHL has been skipped.
inline bool decode(uint8_t *p, size_t size, Message &m,
                   uint32_t &receivedCrc, uint32_t &computedCrc) {
  if (size < kHeader) return false;
  const uint16_t length = read16(p + 12);
  if (length == 0 || length > kMaxPayload || size != kHeader + length)
    return false;
  receivedCrc = read32(p + 16);
  write32(p + 16, 0);
  computedCrc = crc32(p, size);
  write32(p + 16, receivedCrc);
  if (computedCrc != receivedCrc || read16(p) != 6400 ||
      read16(p + 2) != 6401 || read32(p + 4) == 0 ||
      read32(p + 8) != 0 || read16(p + 14) != 0) return false;
  m.length = length;
  for (size_t i = 0; i < length; ++i) m.payload[i] = p[kHeader + i];
  return true;
}
inline bool verify(const Message &m, uint32_t expected) {
  if (expected > kMessages || m.length != (expected == kMessages ? 17 : kMaxPayload)
      || read32(m.payload) != expected) return false;
  for (size_t j = 4; j < m.length; ++j)
    if (m.payload[j] != (expected + j) % 256) return false;
  return true;
}
}  // namespace a4
