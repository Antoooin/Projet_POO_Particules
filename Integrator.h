#pragma once
#include "System.h"

class Integrator {
public:
    virtual ~Integrator() = default;

    virtual void step(System& system, double dt) = 0;
};
