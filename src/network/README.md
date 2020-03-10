# Network

This is a small wrapper around sockets to make them easier to manage.

## Warnings

Be aware of the `REUSEADDR` options in the socket servers. Sometimes previous runs will still have their servers up, causing new instances to attempt to use those and deadlocks the entire application.
