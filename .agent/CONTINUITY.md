[PLANS]
- 2026-11-24T16:16:07Z [USER] Implement Assignment 4 Q1 AP and separately selectable transmitter; group 10. Create data/q1 through q5 for evidence. Upload not requested.

[DECISIONS]
- 2026-11-24T16:16:07Z [USER] Use custom transport over raw IPv4 protocol 253, no TCP/UDP. Supersedes previous TCP-client task.
- 2026-11-24T16:16:07Z [CODE] PlatformIO ap (default) and transmitter environments select roles in src/main.cpp; shared packet codec in include/transport.h. SSID CS640-A4-10; AP 192.168.4.1/24, DHCP; Step 1 only, no ACKs.

[PROGRESS]
- 2026-11-24T16:16:07Z [CODE] Implemented CRC, IHL parsing, eight-slot queue, separate tasks, predictable 20 messages, two-second first delay, 100 ms send spacing, decoded header and application verification logs. Added data/q1–q5 evidence guides and unmeasured Q5 CSV template.

[DISCOVERIES]
- 2026-11-24T16:16:07Z [TOOL] Arduino softAPConfig enables AP; initialize stack in STA mode and create socket/tasks before AP configuration. Cppcheck 2.17.1 default branch limit returned nonzero; exhaustive analysis passes.

[OUTCOMES]
- 2026-11-24T16:16:07Z [TOOL] Both container builds, cppcheck role variants, C++ packet tests, and diff whitespace checks passed. Summary and source hashes in data/q1/software-validation.txt. Assignment PDF reread against implementation/checklists.
- 2026-11-24T16:16:07Z [CODE] No board flashed; hardware evidence and Q1 report answer still require real board runs. Q2–Q5 firmware not implemented. Existing report unchanged. Previous TCP source backed up to /tmp/assignment4-previous-tcp.cpp.
