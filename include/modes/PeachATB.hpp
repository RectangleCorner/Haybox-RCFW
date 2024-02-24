#ifndef _MODES_PEACHATB_HPP
#define _MODES_PEACHATB_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

typedef struct {
    bool crouch_walk_os = false;
} PeachATBOptions;

class PeachATB : public ControllerMode {
  public:
    PeachATB(
        socd::SocdType horizontal_socd,
        socd::SocdType vertical_socd,
        PeachATBOptions options = {}
    );

  protected:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);

  private:
    PeachATBOptions _options;
    bool _horizontal_socd;

    void HandleSocd(InputState &inputs);
};

#endif
