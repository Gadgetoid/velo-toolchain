# debugmgr protocol

debugmgr reads requests from velo-emu's host mailbox and sends one reply to each. velo-emu delivers each message whole, framed on its `--agent` socket as a u32 little-endian length followed by the message.

All integers are little-endian. Strings are a u16 count of UTF-16 code units followed by that many UTF-16LE code units, with no terminator. Paths are CE paths, `\` separated and absolute. Fields aren't aligned.

## Messages

Request: u16 command, u16 sequence, payload.

Reply: u16 command | 0x8000, u16 sequence (copied from the request), u32 status, payload. The payload is empty unless status is 0.

Sequences 0x0001 to 0x7FFF belong to socket clients such as `velo-debug`. 0x8000 to 0xFFFF are reserved for velo-emu itself (its GDB stub), so it can share the mailbox and pick out its own replies.

A request must fit in the mailbox's maximum message size, which PING reports.

## Status

| Status | Meaning |
| --- | --- |
| 0 | done |
| 0x20000001 | unknown command |
| 0x20000002 | malformed request |
| 0x20000003 | no such process: KILL only knows programs started by RUN |
| 0x20000004 | too many running programs (16) |
| other | the Windows error from `GetLastError`, e.g. 2 file not found, 3 path not found, 5 access denied, 80 or 183 exists, 112 disk full |

## Commands

| # | Command | Request payload | Reply payload |
| --- | --- | --- | --- |
| 1 | PING | none | u32 protocol version (1), u32 maximum message size, u32 `_WIN32_WCE` (100 or 200) |
| 2 | WRITE | u32 offset, u32 flags, string path, data to the end of the message | none |
| 3 | READ | u32 offset, u32 length, string path | u32 file size, then up to length bytes from offset |
| 4 | RUN | string program, string arguments (empty for none) | u32 process ID |
| 5 | KILL | u32 process ID | none |
| 6 | LIST | string pattern, e.g. `\Windows\*` | u32 count, then per entry: u32 attributes, u32 size, string name |
| 7 | DELETE | string path | none |
| 8 | MKDIR | string path | none |
| 9 | RMDIR | string path | none |
| 10 | MOVE | string from, string to | none |
| 11 | QUIT | none | none, then debugmgr exits |
| 12 | HANDOVER | as RUN | as RUN, then debugmgr exits |

WRITE flag 0x1 creates the file, or truncates it if it exists. Without it the file must exist. Data is written at offset.

READ returns fewer bytes than asked at the end of the file, or when the reply would exceed the maximum message size.

The process ID is CE's `dwProcessId` from `CreateProcessW`.

LIST stops early if the reply would exceed the maximum message size.

## GDB's file and run requests

How velo-emu's GDB stub can serve GDB's file transfer and extended-remote run through debugmgr:

| GDB | debugmgr |
| --- | --- |
| `vFile:open` with `O_CREAT` or `O_TRUNC` | WRITE at offset 0 with flag 0x1 and no data; keep the path for the descriptor |
| `vFile:open` otherwise | READ length 0 to check it exists; keep the path |
| `vFile:pwrite` | WRITE at the offset, no flags |
| `vFile:pread` | READ |
| `vFile:close` | forget the descriptor |
| `vFile:unlink` | DELETE |
| `vRun` | RUN; debug the new process and stop at its first instruction |
| `vKill` | KILL |

Windows errors map to GDB's errno values: 2 and 3 to ENOENT, 5 to EACCES, 80 and 183 to EEXIST, 112 to ENOSPC, the rest to EIO.
