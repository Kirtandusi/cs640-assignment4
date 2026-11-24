# Assignment 4 — Question 1 (group 10)

Heltec WiFi LoRa 32 V3 firmware for Step 1 only. Both board roles live in this
project: select the `ap` or `transmitter` PlatformIO environment. The default is
`ap`. Uploading one environment replaces the firmware on the selected board;
run the two roles on separate boards.

This implements a custom transport over **raw IPv4 protocol 253**, not TCP or
UDP. The stack adds the IPv4 header. The receiver skips IHL × 4 bytes, validates
the exact transport length, CRC-32/IEEE, and DATA header fields, and delivers
port 6401 payloads to an eight-slot FreeRTOS queue. Separate application and
transport tasks run on each board. Step 1 has no ACKs or retransmissions.

## Configuration and execution

- SSID: `CS640-A4-10`; password: `cs640wifi` (change both roles together).
- AP address: `192.168.4.1/24`; client gets its address through DHCP.
- Edit `kRun` in `src/main.cpp` before building both boards for a recorded run.
- Firmware identifier: `a4-q1-v2`. N=20; application delay=0; no injected faults.
- DATA ports: source 6400, destination 6401. Sequences start at 1; ACK/window=0.
- Messages 1–19 contain 1460 bytes; message 20 contains 17 bytes, totaling 27,757.
- First send waits two seconds after association/IP acquisition; later sends
  have a 100 ms delay between attempts. The AP creates its socket, queue, and
  tasks before publishing the SSID, establishing receiver readiness.

Build and test with the container command in `AGENTS.md`. Use host PlatformIO
for USB operations (replace each port with the correct board's device):

```sh
pio device list
pio run -e ap -t upload --upload-port /dev/cu.AP_PORT
pio device monitor -e ap --port /dev/cu.AP_PORT --baud 115200
```

In another terminal, upload and monitor the transmitter:

```sh
pio run -e transmitter -t upload --upload-port /dev/cu.TRANSMITTER_PORT
pio device monitor -e transmitter --port /dev/cu.TRANSMITTER_PORT --baud 115200
```

In VS Code, the equivalent actions are under PlatformIO **Project Tasks → ap**
or **transmitter → Upload**. Use explicit ports when both boards are connected.
Start/reset the AP first, attach both serial monitors, and then reset the
transmitter to capture the complete run. Reset both boards for a fresh run.
No board is flashed by the build/test commands.

## Q1 evidence to collect on hardware

Save unedited logs from both boards with the same run ID. The firmware prints
local monotonic timestamps; do not subtract timestamps from different boards.

1. AP `DATA HEADER hex` prints each actual received 20-byte header.
   `DECODE` prints every field and received/recomputed CRC with PASS.
2. AP `APP delivery number=20 ... length=17 ... verify=PASS` confirms the short
   payload. `APP SUMMARY` must show 20 messages, 27,757 bytes and PASS.
   The application checks message numbers, order, length, and every byte.
3. AP `APP WAIT` precedes the first delivery and its measured `wait_ms`.
   The application sleeps in `xQueueReceive(messages, &message, portMAX_DELAY)`
   in `applicationTask`; the transmitter logs its initial 2000 ms delay.
   AP wait time can be longer because it includes startup/association time.

Builds and host packet tests do not establish hardware delivery. Actual Q1
observations and report claims must come from the two-board run. Later
checkpoints (ACKs, Go-Back-N, flow control) are not implemented here.

Evidence folders and per-question collection checklists are in `logs/q1/`
through `logs/q5/`. The Q5 CSV is a configuration template until measured.

Serial Monitor automatically saves each session to `logs/q1/<environment>-<UTC timestamp>.log`. Restart an existing monitor to activate the file-saving filter. Upload v2 firmware to both boards to enable transcript replay and expanded evidence logging. The save path appears when the monitor starts.

## Recovering a run when the monitor opened late

Firmware v2 stores up to 48 KiB of serial evidence in RAM, starting at boot.
After the transfer, type `l` (lowercase L) in each board's monitor. It replays
that board's transcript into the automatically saved file, including original
board timestamps. Replay markers distinguish this copy from live events;
do not count replayed lines as new packet arrivals. `truncated=no` confirms
that all emitted evidence was retained. A reset/power loss clears this buffer.
The monitor must be connected to save anything to the computer.

Type `r` to restart a board. Save/replay existing evidence first, then restart
AP and wait for AP READY before restarting the transmitter. A run is not
silently reset upon DATA 1: unexpected extra messages generate RUN INVALID.
Use both boards' PASS evidence from the same fresh run; an old failure cannot
be repaired by replaying it. The expanded log adds header/CRC output for every
packet, enqueue outcomes, explicit wait start/delivery times, measured initial
delay, and the successful send count. Log lines are serialized across tasks.

These commands require uploading the updated firmware. They do not exist in
v1. No ACK, retransmission, or later-checkpoint behavior has been added.
