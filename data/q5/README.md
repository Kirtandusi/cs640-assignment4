# Q5 — final evaluation

Status: checkpoint not implemented; measurements pending. `trials.csv` contains
nine configuration rows only; blank result fields mean unmeasured.

Run F (0 ms application delay, no loss), S (100 ms, no loss), and L (0 ms,
independent 5% DATA-arrival loss including retransmissions), three times each.
N=512; payload bytes=746077; queue/sender slots=8; both timers=500 ms. Keep board
positions and Wi-Fi settings fixed. Reset both boards for every trial. Disable
per-packet printing and other fault hooks. Record PRNG and a distinct seed for
each L run. Drop before transport state changes; count injected drops separately.

For each run save `ap.log`, `transmitter.log`, configuration/code version, and
summary counters. Fill `trials.csv` only from actual measurements. AP duration
starts at first DATA acceptance and ends after verification of the last payload.
Success requires valid sender ACK 512 and all 512 messages received in order,
exactly once, with correct bytes. Goodput=746077/Tapp in bytes/s for successful
runs only. For failures use N/A goodput, elapsed time, and partial counts; stop
at 180 seconds from first send. Do not derive timings across board clocks.

Save analysis script/calculations and a summary table: for F/S/L report success
count out of 3, median and min–max successful goodput (N/A if none), median DATA
retransmissions over all 3 trials, and largest queue occupancy. Include integrity
checks and expected-new-DATA full-queue discards. Explain observed performance,
including similar results if seen, and cite one actual bug fix or correctness
check. Include one photo of both boards under `data/`.
