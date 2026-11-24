# Q2 — pipelining and Go-Back-N

Status: checkpoint not implemented; hardware evidence pending.

Save separate run directories for normal delivery, first-copy DATA 3 loss,
and first ACK 20 corruption. Reset both boards per run. N=20, sender slots=8,
receiver slots=8, fixed advertised Window=4, oldest-outstanding timer=500 ms.
Remove Step 1's 100 ms send delay. Corrupt ACK 20 by XORing header byte 15 with
0x01 without recalculating CRC; no other faults in that run.

Each directory needs unedited `ap.log`, `transmitter.log`, and annotated
`notes.md`. Identify the concurrent outstanding packets; repeated ACK 2,
out-of-order discards, timeout and retransmitted range after losing DATA 3;
and received/recomputed CRC of the bad ACK, rejection before sender-state
changes, and retransmission without duplicate application delivery. Compare
500 ms with timestamps from one board. Show valid ACK 20 and exactly 20 verified
application deliveries. Preserve the code version used for these runs.
