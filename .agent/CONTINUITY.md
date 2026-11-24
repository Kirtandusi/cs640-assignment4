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

- 2026-11-24T16:33:36Z [USER] Requested automatic serial log saving during live reception.
- 2026-11-24T16:33:36Z [CODE] Evidence directory now logs/ (supersedes data/ paths above). Configured q1_log monitor filter in platformio.ini; monitor/filter_q1_log.py writes unique role/UTC files under logs/q1 and flushes every receive chunk. Existing monitors require restart; earlier output cannot be recovered automatically. Firmware unchanged.
- 2026-11-24T16:35:17Z [TOOL] Reviewed entire logs/q1/ap-20261124T163355.884120Z.log: 20 CRC-passing RX packets, but app expected 21–40 for messages 1–20 and all verification FAIL. Missing startup/header decode and transmitter log. AP state was not reset for this transfer; fresh capture with both boards reset required for Q1 evidence.
- 2026-11-24T16:39:57Z [USER] Requested fuller logging/code changes to obtain Q1 evidence.
- 2026-11-24T16:39:57Z [CODE] Firmware a4-q1-v2 adds mutex-serialized lines, 48 KiB RAM transcript with serial l replay/r restart, every DATA header/CRC, enqueue outcome, explicit wait timing, initial delay measurement, successful sends and extra-delivery warning. No automatic run reset or transport changes.
- 2026-11-24T16:39:57Z [TOOL] Both v2 builds, cppcheck role variants and packet tests passed. Saved software-validation-v2.txt. Firmware not flashed; on-board replay and fresh Q1 evidence pending. Both boards require v2 upload. RAM transcript does not survive reset.
- 2026-11-24T16:40:56Z [USER] Requested Git conflict resolution and push. Merged origin/main rename commit with local v2 commits; resolved logs/q1/README.md retaining expanded capture/replay instructions. No firmware merge changes.
