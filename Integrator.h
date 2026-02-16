#pragma once
#include "System.h"
#include "Potential.h"

class Integrator {
public:
    virtual ~Integrator() = default;

    virtual void step(System& system,
                      Potential& potential,
                      double dt) = 0;
};
