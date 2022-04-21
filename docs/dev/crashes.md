# Crashes

The codebase makes use of exceptions with embedded stacktraces whenever possible. These traces are dumped into a file called `crash.txt` next to the binary whenever an unhandled exception results in a crash (or when an assertion is triggered for debug builds).

These stacktraces are very useful for debugging purposes, and should be provided in issues, if a bug is reproducible.
