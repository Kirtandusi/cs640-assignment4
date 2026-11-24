#include "transport.h"
#include <assert.h>
#include <stdio.h>

int main() {
  const uint8_t vector[] = "123456789";
  assert(a4::crc32(vector, 9) == 0xcbf43926);
  uint8_t packet[1481]{};
  a4::Message original{}, decoded{};
  uint32_t received = 0, computed = 0;
  for (uint32_t sequence = 1; sequence <= 20; ++sequence) {
    a4::generate(original, sequence);
    const size_t size = a4::encode(packet, original);
    assert(size == (sequence == 20 ? 37 : 1480));
    assert(packet[0] == 0x19 && packet[1] == 0 && packet[2] == 0x19 && packet[3] == 1);
    assert(a4::read32(packet + 4) == sequence);
    assert(a4::decode(packet, size, decoded, received, computed));
    assert(received == computed && a4::verify(decoded, sequence));
    assert(!a4::verify(decoded, sequence + 1));
    assert(!a4::decode(packet, size - 1, decoded, received, computed));
    assert(!a4::decode(packet, size + 1, decoded, received, computed));
    packet[20] ^= 1;
    assert(!a4::decode(packet, size, decoded, received, computed));
    packet[20] ^= 1;
    packet[15] ^= 1;
    assert(!a4::decode(packet, size, decoded, received, computed));
  }
  a4::generate(original, 1);
  const size_t size = a4::encode(packet, original);
  a4::write16(packet + 2, 6402);
  a4::write32(packet + 16, 0);
  a4::write32(packet + 16, a4::crc32(packet, size));
  assert(!a4::decode(packet, size, decoded, received, computed));
  assert(!a4::decode(packet, 19, decoded, received, computed));
  puts("PASS: CRC vector, 20 payloads, lengths, order, corruption, wrong port, truncated header");
}
