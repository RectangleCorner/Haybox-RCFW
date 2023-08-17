
#include "modes/Smash64.hpp"

#define ANALOG_STICK_MIN 48
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 208

Smash64::Smash64(socd::SocdType horizontal_socd, socd::SocdType vertical_socd) {
    _socd_pair_count = 2;
    _socd_pairs = new socd::SocdPair[_socd_pair_count]{
        socd::SocdPair{&InputState::left,   &InputState::right, horizontal_socd         },
        socd::SocdPair{ &InputState::down,  &InputState::up,  vertical_socd           },
        //If using WASD, uncomment the below line and comment the above line
        //socd::SocdPair{ &InputState::down,   &InputState::w, vertical_socd        },
    };
}

void Smash64::UpdateDigitalOutputs(InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.a;
    outputs.b = inputs.b;

    outputs.y = inputs.mod_y;

    outputs.buttonR = inputs.l; //shield
    if (inputs.nunchuk_connected) {
        outputs.triggerLDigital = inputs.nunchuk_z;
    } else {
        outputs.triggerLDigital = inputs.r; //taunt
    }
    outputs.triggerRDigital = inputs.z; //grab

    outputs.start = inputs.home; //bubblebox start

     // Activate D-Pad layer by holding Mod X + Mod Y or Nunchuk C button. 
    // Remove inputs.w if using that as Up.
    if ((inputs.mod_x && inputs.mod_y) || inputs.nunchuk_c || inputs.w) {
        outputs.dpadUp = inputs.c_up;
        outputs.dpadDown = inputs.c_down;
        outputs.dpadLeft = inputs.c_left;
        outputs.dpadRight = inputs.c_right;
    }

    if (inputs.select)
        outputs.dpadLeft = true;
    if (inputs.home)
        outputs.dpadRight = true;
}

void Smash64::UpdateAnalogOutputs(InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.left,
        inputs.right,
        inputs.down,
        inputs.up, // replace with inputs.w if using WASD
        inputs.c_left,
        inputs.y, //y now acts as c-right
        inputs.mod_x, //mod X now acts as c-down
        inputs.x, // X now acts as c-up
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    bool shield_button_pressed = inputs.z || inputs.r;
    if (directions.diagonal) {
        // q1/2 = 7000 7000
        outputs.leftStickX = 128 + (directions.x * 56);
        outputs.leftStickY = 128 + (directions.y * 56);
        // L, R, LS, and MS + q3/4 = 7000 6875 (For vanilla shield drop. Gives 44.5
        // degree wavedash). Also used as default q3/4 diagonal if crouch walk option select is
        // enabled.
        if (directions.y == -1 && (shield_button_pressed)) {
            outputs.leftStickX = 128 + (directions.x * 56);
            outputs.leftStickY = 128 + (directions.y * 55);
        }
    }



    // Shut off C-stick when using D-Pad layer.
    if ((inputs.mod_x && inputs.mod_y) || inputs.nunchuk_c) {
        outputs.rightStickX = 128;
        outputs.rightStickY = 128;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}