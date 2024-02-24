#ifndef _MODES_PEACHPOTION_HPP
#define _MODES_PEACHPOTION_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

typedef struct {
    bool crouch_walk_os = false;
} PeachPotionOptions;

class PeachPotion : public ControllerMode {
  public:
    PeachPotion(
        socd::SocdType horizontal_socd,
        socd::SocdType vertical_socd,
        PeachPotionOptions options = {}
    );

  protected:
    void UpdateDigitalOutputs(InputState &inputs, OutputState &outputs);
    void UpdateAnalogOutputs(InputState &inputs, OutputState &outputs);

  private:
    PeachPotionOptions _options;
    bool _horizontal_socd;

    void HandleSocd(InputState &inputs);
};

#endif
