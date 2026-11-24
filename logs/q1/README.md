# Q1 — raw protocol 253 and application delivery

Status: firmware implemented; two-board evidence pending.

Required run: N=20, eight receive slots, prompt application reads, no ACKs,
100 ms between DATA sends, first send delayed 2000 ms. Messages 1–19 have
1460-byte payloads; message 20 has 17 bytes. Expected total: 27,757 bytes.

Save in a run-ID subdirectory:
- `ap.log`: startup/IP, CRC self-test, actual DATA header hex and decoded fields,
  received/recomputed CRC, APP WAIT and subsequent delivery, all 20 verifications,
  final 17-byte message, and summary.
- `transmitter.log`: startup/IP, two-second initial delay, all 20 send results.
- `notes.md`: cite exact log lines for header decoding, final short payload, and
  blocking wait. Cite `applicationTask` in `src/main.cpp` and its
  `xQueueReceive(messages, &message, portMAX_DELAY)` call. Explain why a blocking
  receive sleeps until transport enqueues a payload. Use AP-local timestamps to
  measure its wait; startup/association can make it exceed two seconds.

`software-validation.txt` records software checks only, not radio measurements.

## Automatic capture

PlatformIO's configured `q1_log` monitor filter saves received serial text to
`logs/q1/<environment>-<UTC timestamp>.log` and flushes every chunk immediately.
Each monitor session creates a new file; earlier sessions are not overwritten.
The monitor prints the full save path when it opens. Select the correct role:

```sh
pio device monitor -e ap --port <AP_PORT>
pio device monitor -e transmitter --port <TRANSMITTER_PORT>
```

Restart already-open monitors once to load this setting. With firmware v2, type `l` after the run to replay boot-to-current evidence
from board RAM into the saved log. v1 cannot replay earlier output. After both
monitors are saving, reset AP first and then transmitter for complete evidence.
Upload v2 to both boards to enable replay and expanded logging. Keep the timestamped files
as the original logs and associate their run ID from the firmware output.


Replay blocks retain original timestamps and are labeled SAVED TRANSCRIPT;
do not count their events twice. Check `truncated=no`. Type `r` to restart a
board after saving its evidence. Reset clears RAM, so replay before resetting.
Require no RUN INVALID lines and a PASS summary for the selected fresh run.
The logs include each received header and CRC, enqueue acceptance, explicit
application wait/delivery times, initial sender delay and send counts.
