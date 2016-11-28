MCC
==========

MCC is a Minecraft Client API written in C. The goal is to implement the entire network protocol from the client point of view. The intended use case was botting, but the possibilities are endless. Bonus points if you can implement

## Documentation and Resources ##

### Protocol Version: 210 ###

[wiki.vg](http://wiki.vg)


## Supported Features
* API functions to send every server-bound packet type
* Receive/parse every client-bound packet type, and allows callbacks to be registered for each packet type
* Packet compression (so please enable it on your server)

## Limitations ##
* We only support the protocol for the most current stable release (for now)
* No authentication/encryption, so the server must be run in offline mode
* Note that this is largely just an implementation of the network protocol. This library is not very aware of game logic and rules

## Upcoming Features ##
* Register a main "idle" loop for the client
* Authentication and encryption
* Support for multiple protocol versions
* Utility functions for basic actions, such as digging, moving, path-finding, etc.
