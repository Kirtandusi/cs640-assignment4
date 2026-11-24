# Assignment 4 evidence

Store actual measurements under the matching question directory. Each question's
README lists the evidence still needed. No hardware results have been collected
by this implementation task. Empty templates are not measurements.

For each run, create a subdirectory named with the run ID (for example
`q1/q1-001/`) and save `ap.log` and `transmitter.log` there. Preserve unedited
serial logs, including failures. Use the same run ID on both boards and record
firmware/commit, roles, IPs, message count, queue capacity, application delay,
timers, and fault settings. Keep annotations separate from raw logs. Include a
photo showing both boards in this directory when available.

Reset both boards for each run (required for Steps 2–5). For Steps 2–5, mark a
run failed if it has not finished within 180 seconds of the first send. Never
subtract timestamps from different boards. Save checkpoint code versions or
modes as subsequent steps are implemented.
