#pragma once
#include "Integrator.h"

class VelocityVerlet : public Integrator {
public:
    void step(System& system,
              Potential& potential,
              double dt) override;
};
