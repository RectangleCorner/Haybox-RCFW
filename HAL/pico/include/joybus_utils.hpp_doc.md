This looks like a header file containing utility functions related to a game console's Joybus interface. The file may include the definition of the `ConnectedConsole` enum class, which represents the possible connected consoles (Gamecube, N64, or None). 

The main function in this file is likely `detect_console()`, which takes a `joybus_pin` parameter and returns a `ConnectedConsole` value representing the detected connected console. The purpose of this function is to determine which console is connected to a specific joybus_pin.

The `#ifndef` and `#define` directives at the beginning of the file suggest that this file has include guards to prevent multiple inclusion in the same compilation unit.