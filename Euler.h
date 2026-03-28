#pragma once
#include "Integrator.h"

class Euler : public Integrator {
public:
    void step(System& system,
              Potential& potential,
              double dt) override;
};