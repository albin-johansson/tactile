# Logging

The application will continuously log information, which can be viewed in the log dock (`View -> Widgets -> Log`). You can also find logs from the most recent session in the persistent file directory used by Tactile. The location of this directory varies by platform (at startup, the location of the persistent file directory is logged and can therefore be viewed the log dock). You can also open the persistent file directory in your file explorer with `Debug -> Open Persistent File Directory...`. You'll find the log file at `logs/tactile_log.txt`.

## Crashes

The codebase makes use of exceptions with embedded stacktraces wherever possible. These stacktraces are hugely beneficial for debugging purposes, and should be provided in GitHub crash report issues.
