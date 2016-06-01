MCC
==========

[![Build Status](https://travis-ci.org/NosotrosNueces/mcc.svg?branch=travis)](https://travis-ci.org/NosotrosNueces/mcc)
[![Gitter](https://badges.gitter.im/Join Chat.svg)](https://gitter.im/NosotrosNueces/mcc?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

MCC is a MineCraft Client API written in C. MCC hopes to aid automation in
Minecraft by providing a framework to program bots. MCC provides some
abstractions and an event-driven interface that makes bot-writing easy.

## Documentation and Resources ##

### Protocol Version: 201 ###

[mcc.lf.lc](http://mcc.lf.lc)

[wiki.vg](http://wiki.vg)

** We only support the protocol for the most current stable release (for now) **

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

## Upcoming Features ##
* Building structures from compressed files in a custom format
* Inventory management
* Support for multiple protocol versions
