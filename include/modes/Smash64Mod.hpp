#ifndef _MODES_SMASH64MOD_HPP
#define _MODES_SMASH64MOD_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Smash64Mod : public ControllerMode {
  public:
    Smash64Mod(socd::SocdType socd_type);

  protected:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);

  private:
    bool _horizontal_socd;

    void HandleSocd(InputState &inputs);
};

#endif
