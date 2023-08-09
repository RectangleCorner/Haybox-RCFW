#ifndef _MODES_SMASH64_HPP
#define _MODES_SMASH64_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class Smash64 : public ControllerMode {
  public:
    Smash64(socd::SocdType socd_type);

  protected:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);

  private:
    void HandleSocd(InputState &inputs);
};

#endif
