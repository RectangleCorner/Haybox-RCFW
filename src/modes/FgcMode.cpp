#include "modes/FgcMode.hpp"

FgcMode::FgcMode(socd::SocdType horizontal_socd, socd::SocdType vertical_socd) {
    _socd_pair_count = 2;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,   &InputState::right, horizontal_socd         },
/*
        Uncomment line 14 if we use A as up and comment out line 13
  */
        socd::SocdPair{ &InputState::down,  &InputState::mod_x, vertical_socd           },
        //socd::SocdPair{ &InputState::down,  &InputState::a,  vertical_socd           },

        //If using WASD, uncomment the below line and comment the above lines
        //socd::SocdPair{ &InputState::down,   &InputState::w, vertical_socd     },
    };
}

void FgcMode::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    // Directions
    outputs.dpadLeft = inputs.left;
    outputs.dpadRight = inputs.right;
    outputs.dpadDown = inputs.down;
    outputs.dpadUp = inputs.mod_x;
    //outputs.dpadUp = inputs.w

    // Menu keys
    outputs.select = inputs.c_left;
    //function layer
    if (inputs.mod_y){
        outputs.home = inputs.start;
    } else {
        outputs.start = inputs.start;
    }
    outputs.leftStickClick = inputs.a;
    outputs.rightStickClick = inputs.c_up;


    // Right hand bottom row
    outputs.a = inputs.b;
    outputs.b = inputs.x;
    outputs.triggerRDigital = inputs.z;
    outputs.triggerLDigital = inputs.up;

    // Right hand top row
    outputs.x = inputs.r;
    outputs.y = inputs.y;
    outputs.buttonR = inputs.lightshield;
    outputs.buttonL = inputs.midshield;
}

void FgcMode::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    outputs.leftStickX = 128;
    outputs.leftStickY = 128;
    outputs.rightStickX = 128;
    outputs.rightStickY = 128;
    outputs.triggerLAnalog = outputs.triggerLDigital ? 255 : 0;
    outputs.triggerRAnalog = outputs.triggerRDigital ? 255 : 0;
}
