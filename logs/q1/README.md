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
