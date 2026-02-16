#pragma once
#include "Potential.h"

class IdealGas : public Potential {
public:
    void computeForces(System& system) override;
};
