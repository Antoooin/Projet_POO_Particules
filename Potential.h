#pragma once
#include "System.h"

class Potential {
public:
    virtual ~Potential() = default;
    virtual void computeForces(System& system) = 0;
    virtual double energy(const System& system) const { return 0.0; }
};
