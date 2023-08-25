#include "modes/WingmanFgcMode.hpp"

WingmanFgcMode::WingmanFgcMode(socd::SocdType horizontal_socd, socd::SocdType vertical_socd) {
    _socd_pair_count = 2;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,   &InputState::right, horizontal_socd         },
/* Mod X override C-Up input if both are pressed. Without this, neutral SOCD doesn't work
  properly if Down and both Up buttons are pressed, because it first resolves Down + Mod X
  to set both as unpressed, and then it sees C-Up as pressed but not Down, so you get an up
  input instead of neutral. 
        Uncomment line 14 if we use c_up as up and comment out line 13
  */
        socd::SocdPair{ &InputState::down,  &InputState::mod_x, vertical_socd           },
        //socd::SocdPair{ &InputState::down,  &InputState::c_up,  vertical_socd           },
    };
}

void WingmanFgcMode::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    // Directions
    outputs.dpadLeft = inputs.left;
    outputs.dpadRight = inputs.right;
    outputs.dpadDown = inputs.down;
    outputs.dpadUp = inputs.mod_x;

    // Menu keys
    outputs.select = inputs.c_left;
    //function layer
   if (inputs.mod_y){
        outputs.home = inputs.home;
    } else {
        outputs.start = inputs.home; //bubblebox start
    }
    outputs.leftStickClick = inputs.a;
    outputs.rightStickClick = inputs.c_up;


    // Right hand bottom row    
    outputs.a = inputs.x;
    outputs.b = inputs.b; // swapped because of a wingman oddity
    outputs.triggerRDigital = inputs.z;
    outputs.triggerLDigital = inputs.up;

    // Right hand top row
    outputs.x = inputs.y;
    outputs.y = inputs.r;
    outputs.buttonR = inputs.lightshield;
    outputs.buttonL = inputs.midshield;
}

void WingmanFgcMode::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    outputs.leftStickX = 128;
    outputs.leftStickY = 128;
    outputs.rightStickX = 128;
    outputs.rightStickY = 128;
    outputs.triggerLAnalog = outputs.triggerLDigital ? 255 : 0;
    outputs.triggerRAnalog = outputs.triggerRDigital ? 255 : 0;
}
