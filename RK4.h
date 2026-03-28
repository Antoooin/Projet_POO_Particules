#pragma once
#include "Integrator.h"

class RK4 : public Integrator {
public:
    void step(System& system,
              Potential& potential,
              double dt) override;
};