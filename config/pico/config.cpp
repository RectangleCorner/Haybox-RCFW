#include "comms/B0XXInputViewer.hpp"
#include "comms/DInputBackend.hpp"
#include "comms/GamecubeBackend.hpp"
#include "comms/N64Backend.hpp"
#include "comms/NintendoSwitchBackend.hpp"
#include "comms/XInputBackend.hpp"
#include "config/mode_selection.hpp"
#include "core/CommunicationBackend.hpp"
#include "core/InputMode.hpp"
#include "core/KeyboardMode.hpp"
#include "core/pinout.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "hardware/timer.h"
#include "input/GpioButtonInput.hpp"
#include "input/NunchukInput.hpp"
#include "joybus_utils.hpp"
#include "modes/FgcMode.hpp"
#include "modes/Melee20Button.hpp"
#include "modes/ProjectM.hpp"
#include "modes/Smash64.hpp"
#include "modes/Ultimate.hpp"
#include "modes/WingmanFgcMode.hpp"
#include "stdlib.hpp"

#include <pico/bootrom.h>

CommunicationBackend **backends = nullptr;
size_t backend_count;
KeyboardMode *current_kb_mode = nullptr;

GpioButtonMapping button_mappings[] = {
    {&InputState::l,            5 },
    { &InputState::left,        4 },
    { &InputState::down,        3 },
    { &InputState::right,       2 },
    { &InputState::mod_x,       6 },
    { &InputState::mod_y,       7 },
    { &InputState::nunchuk_c,   8 },
    { &InputState::select,      10},
    { &InputState::start,       0 },
    { &InputState::home,        11},
    { &InputState::w,           1 },
    { &InputState::c_left,      13},
    { &InputState::c_up,        12},
    { &InputState::c_down,      15},
    { &InputState::a,           14},
    { &InputState::c_right,     16},

    { &InputState::b,           26},
    { &InputState::x,           21},
    { &InputState::z,           19},
    { &InputState::up,          17},

    { &InputState::r,           27},
    { &InputState::y,           22},
    { &InputState::lightshield, 20},
    { &InputState::midshield,   18},
};
size_t button_count = sizeof(button_mappings) / sizeof(GpioButtonMapping);

const Pinout pinout = { .joybus_data = 28,
                        .mux = -1,
                        .nunchuk_detect = -1,
                        .nunchuk_sda = -1,
                        .nunchuk_scl = -1,
                        .rumble = 23,
                        .rumbleBrake = 29 };

void setup() {
    // Create GPIO input source and use it to read button states for checking button holds.
    GpioButtonInput *gpio_input = new GpioButtonInput(button_mappings, button_count);

    InputState button_holds;
    gpio_input->UpdateInputs(button_holds);

    // Bootsel button hold as early as possible for safety.
    if (button_holds.start) {
        reset_usb_boot(0, 0);
    }

    // Turn on LED to indicate firmware booted.
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    gpio_init(pinout.rumble);
    gpio_init(pinout.rumbleBrake);
    gpio_set_dir(pinout.rumble, GPIO_OUT);
    gpio_set_dir(pinout.rumbleBrake, GPIO_OUT);
    gpio_set_function(pinout.rumble, GPIO_FUNC_PWM);
    gpio_set_function(pinout.rumbleBrake, GPIO_FUNC_PWM);
    const uint rumbleSlice_num = pwm_gpio_to_slice_num(pinout.rumble);
    const uint brakeSlice_num = pwm_gpio_to_slice_num(pinout.rumbleBrake);
    pwm_set_wrap(rumbleSlice_num, 255);
    pwm_set_wrap(brakeSlice_num, 255);
    pwm_set_chan_level(rumbleSlice_num, PWM_CHAN_B, 0); // B for odd pins
    pwm_set_chan_level(brakeSlice_num, PWM_CHAN_B, 255); // B for odd pins
    pwm_set_enabled(rumbleSlice_num, true);
    pwm_set_enabled(brakeSlice_num, true);

    // Create array of input sources to be used.
    static InputSource *input_sources[] = { gpio_input };
    size_t input_source_count = sizeof(input_sources) / sizeof(InputSource *);

    ConnectedConsole console = detect_console(pinout.joybus_data);

    /* Select communication backend. */
    CommunicationBackend *primary_backend;
    if (console == ConnectedConsole::NONE) {
        if (button_holds.x) {
            // If no console detected and X is held on plugin then use Switch USB backend.
            NintendoSwitchBackend::RegisterDescriptor();
            backend_count = 1;
            primary_backend = new NintendoSwitchBackend(input_sources, input_source_count);
            backends = new CommunicationBackend *[backend_count] { primary_backend };
            // Default to Wingman FGC mode upon plugin to Brook Wingman.
            primary_backend->SetGameMode(new WingmanFgcMode(socd::SOCD_NEUTRAL, socd::SOCD_NEUTRAL)
            );
            return;
        } else if (button_holds.z) {
            // If no console detected and Z is held on plugin then use DInput backend.
            TUGamepad::registerDescriptor();
            TUKeyboard::registerDescriptor();
            backend_count = 2;
            primary_backend = new DInputBackend(input_sources, input_source_count);
            backends = new CommunicationBackend *[backend_count] {
                primary_backend, new B0XXInputViewer(input_sources, input_source_count)
            };
        } else {
            // Default to XInput mode if no console detected and no other mode forced.
            backend_count = 2;
            primary_backend = new XInputBackend(input_sources, input_source_count);
            backends = new CommunicationBackend *[backend_count] {
                primary_backend, new B0XXInputViewer(input_sources, input_source_count)
            };
            primary_backend->SetGameMode(
                new Melee20Button(
                    socd::SOCD_NEUTRAL,
                    socd::SOCD_2IP_NO_REAC,
                    { .crouch_walk_os = true }
                )
                // new FgcMode(socd::SOCD_NEUTRAL, socd::SOCD_NEUTRAL)
                /*
                new ProjectM(
                    socd::SOCD_2IP_NO_REAC,
                    { .true_z_press = false, .ledgedash_max_jump_traj = true }
                )
                */
            );
        }
    } else {
        if (console == ConnectedConsole::GAMECUBE) {
            primary_backend = new GamecubeBackend(
                input_sources,
                input_source_count,
                pinout.joybus_data,
                pinout.rumble,
                pinout.rumbleBrake
            );
            primary_backend->SetGameMode(
                new Melee20Button(
                    socd::SOCD_NEUTRAL,
                    socd::SOCD_2IP_NO_REAC,
                    { .crouch_walk_os = true }
                )
                // new Ultimate(socd::SOCD_2IP)
                /*
                new ProjectM(
                    socd::SOCD_2IP_NO_REAC,
                    { .true_z_press = false, .ledgedash_max_jump_traj = true }
                )
                */
            );
        } else if (console == ConnectedConsole::N64) {
            primary_backend = new N64Backend(input_sources, input_source_count, pinout.joybus_data);
            primary_backend->SetGameMode(new Smash64(socd::SOCD_NEUTRAL, socd::SOCD_NEUTRAL));
        }

        // If console then only using 1 backend (no input viewer).
        backend_count = 1;
        backends = new CommunicationBackend *[backend_count] { primary_backend };
    }
}

void loop() {
    select_mode(backends[0]);

    for (size_t i = 0; i < backend_count; i++) {
        backends[i]->SendReport();
    }

    if (current_kb_mode != nullptr) {
        current_kb_mode->SendReport(backends[0]->GetInputs());
    }
}

/* Nunchuk code runs on the second core */
NunchukInput *nunchuk = nullptr;

void setup1() {
    while (backends == nullptr) {
        tight_loop_contents();
    }

    // Create Nunchuk input source.
    nunchuk = new NunchukInput(Wire, pinout.nunchuk_detect, pinout.nunchuk_sda, pinout.nunchuk_scl);
}

void loop1() {
    if (backends != nullptr) {
        nunchuk->UpdateInputs(backends[0]->GetInputs());
        busy_wait_us(50);
    }
}
