MCC
==========
[![Gitter](https://badges.gitter.im/Join Chat.svg)](https://gitter.im/NosotrosNueces/mcc?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

MCC is a MineCraft Client API written in C. Its purpose is to enable automation in Minecraft with high level functions and event-driven responses.

[![Build Status](https://travis-ci.org/NosotrosNueces/mcc.svg?branch=travis)](https://travis-ci.org/NosotrosNueces/mcc)

## Documentation and Resources ##
[wiki.vg](http://wiki.vg)

[mcc.lf.lc](http://mcc.lf.lc)

## Packet Format Guide ##

Structs can be serialized to bytes by defining a format string for the packet serializer. The following table contains the available element types:

| fmt | type     |
| --- | -------- |
| `b` | 1 byte   |
| `h` | 2 bytes  |
| `w` | 4 bytes  |
| `l` | 8 bytes  |
| `q` | 16 bytes |
| `s` | string   |
| `v` | varint32 |

In addition, the format language supports the following control structures:

| fmt | type |
| --- | ---- |
| `*` | primitive array |
| `*()` | struct array |
| `[|]` | choice block |

Each control structure uses the value of the preceding primitive element as its count. Each member of a choice block or struct array is read as a separate format string; as such control structures can be nested (but please use caution).
