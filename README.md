MCC
==========

[![Build Status](https://travis-ci.org/NosotrosNueces/mcc.svg?branch=travis)](https://travis-ci.org/NosotrosNueces/mcc)
[![Gitter](https://badges.gitter.im/Join Chat.svg)](https://gitter.im/NosotrosNueces/mcc?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

MCC is a MineCraft Client API written in C. MCC hopes to aid automation in
Minecraft by providing a framework to program bots. MCC provides some
abstractions and an event-driven interface that makes bot-writing easy.

## Documentation and Resources ##
[mcc.lf.lc](http://mcc.lf.lc)

[wiki.vg](http://wiki.vg)

See the
[sample folder](https://github.com/NosotrosNueces/mcc/tree/master/sample) for
example bots.


## Getting Started ##
Cloning the repo and running make should get you up and running::

    $ git clone git@github.com:NosotrosNueces/mcc.git
    $ cd mcc
    $ make
    $ ./mcc

The sample binary has a few options::

    $ ./mcc [<SERVER> [<PORT>]]

where SERVER can be an IP address or a host defined in your hosts file, e.g.
`localhost`. The default server is `localhost`, and the default port is the
default Minecraft port, 25565.


## Packet Format Guide ##

Structs can be serialized to bytes by defining a format string for the packet serializer. The following table describes the format guidelines.

| fmt | type     |
| --- | -------- |
| `b` | 1 byte   |
| `h` | 2 bytes  |
| `w` | 4 bytes  |
| `l` | 8 bytes  |
| `q` | 16 bytes |
| `s` | string   |
| `v` | varint32 |
| `*` | array    |

Note: the array format character is a prefix to other types (e.g. `*b` for a char array).


## Upcoming Features ##
* Building structures from compressed files in a custom format
* Inventory management
