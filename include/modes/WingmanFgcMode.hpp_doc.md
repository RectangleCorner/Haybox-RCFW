Based on the provided file, it seems to be a header file defining a class called `WingmanFgcMode`. This class inherits from a class called `ControllerMode` and provides implementations for two member functions: `UpdateDigitalOutputs` and `UpdateAnalogOutputs`. The file also includes other header files such as `socd.hpp` and `state.hpp`.

Here are some possible assumptions about the content of other files:

1. `core/ControllerMode.hpp`: This file may define the base class `ControllerMode` which `WingmanFgcMode` inherits from.

2. `core/socd.hpp`: This file likely contains definitions for the `SocdType` enum or class, which is used as an argument in the constructor of `WingmanFgcMode`.

3. `core/state.hpp`: This file may define the `InputState` and `OutputState` classes, which are used as arguments in the member functions `UpdateDigitalOutputs` and `UpdateAnalogOutputs`.

To get a clearer understanding of the functionality of `WingmanFgcMode`, it would be helpful to examine the contents of these included files as well.