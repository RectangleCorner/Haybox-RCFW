This code appears to be a header file for a class called `ControllerMode`, which is a subclass of `InputMode`. It includes other header files such as `"core/InputMode.hpp"`, `"core/socd.hpp"`, and `"core/state.hpp"`. 

The `ControllerMode` class has several member functions, including a constructor, `UpdateOutputs`, `ResetDirections`, and `UpdateDirections`. The `UpdateOutputs` function takes in an `InputState` object and an `OutputState` object as parameters, while the `ResetDirections` function has no parameters. The `UpdateDirections` function takes in various boolean and uint8_t parameters, as well as an `OutputState` object.

The `ControllerMode` class also has two private pure virtual functions, `UpdateDigitalOutputs` and `UpdateAnalogOutputs`, which appear to be intended to be implemented by derived classes.

Based on this information, it seems that this file is defining a base class for controller modes, providing some common functionality that can be overridden or extended in derived classes.