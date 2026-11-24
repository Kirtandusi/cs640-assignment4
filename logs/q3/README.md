# Q3 — receive-window flow control

Status: checkpoint not implemented; hardware evidence pending.

Save fast and slow run directories, each with `ap.log`, `transmitter.log`, and
`notes.md`. N=32, queue capacity=8, no injected faults; application delay is
0 ms or 100 ms. Record any initial pause used to fill all eight receive slots.

Capture queue occupancy Q and advertised Window W=8-Q with receiver-local
timestamps, ACK events, application reads, and sender limit R changes. Save
`queue.csv` with columns `run_id,receiver_ms,event,ack,Q,W` and a plotting script
plus exported plot showing both filling and draining. Keep source log line
references in notes.

Annotate one ACK before its corresponding application read, a Window=0 event,
and resumption after space opens. Select a sender ACK (a,w), show resulting
R=max(R,a+w), and identify newly allowed sequence numbers. Compare fast/slow
runs and explain prevention of overflow. Show ACK 32 and exactly 32 verified
messages. Retain the checkpoint code version.
