# Lab 4 — section 1.1 AP

Target: Heltec WiFi LoRa 32 V3. Firmware identifier: `ap-1.1`.

The SSID is `CS640-A4-10` for group 10. The chosen Wi-Fi password is `cs640lab4`; configure the
client with the same SSID and password.

Upload with PlatformIO's Upload action, then open Serial Monitor at 115200
baud and reset the board. The AP uses `192.168.4.1/24`, channel 1, and permits
one client. The ESP32 soft-AP DHCP server assigns the client's IP; the AP logs
that address when assigned. The client firmware must also print its own IP.

This checkpoint only establishes Wi-Fi. It does not implement the protocol-253
transport receiver, CRC, or application queue from sections 1.2–1.6. Do not
start application DATA until that receiver is implemented and ready.

Verification on hardware: confirm the SSID appears, connect the second board
using DHCP, and save the serial output showing AP and client IPs. Compilation
and static checks do not verify radio operation.
