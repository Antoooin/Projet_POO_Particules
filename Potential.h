#pragma once
#include "System.h"

class Potential {
public:
    virtual ~Potential() = default;
    virtual void computeForces(System& system) = 0;
};
