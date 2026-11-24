# Q4 — recovery from one lost window update

Status: checkpoint not implemented; hardware evidence pending.

N=32, eight slots, 500 ms periodic window updates, all other faults disabled.
Save unedited `ap.log`, `transmitter.log`, and annotated `notes.md` in a run-ID
directory. Preserve the checkpoint code version and manual-read controls used.

Required trace in order:
1. Pause application reads; fill queue; sender receives ACK (8,0), with no
   outstanding DATA.
2. Read exactly one message; drop the first ACK (8,1) at the sender. Keep reads
   paused so another application read cannot generate the recovery update.
3. Automatic periodic ACK (8,1) arrives; sender transmits DATA 9; its acceptance
   stops repeats for Z=8. Do not reset or manually send a recovery packet.
4. Resume application reads and finish: valid ACK 32 and 32 verified messages.

Explain why unchanged ACK number 8 plus Window 1 permits DATA 9. Cite timestamps
and read events proving a periodic repeat, not another read, broke the stall.
