#include "IdealGas.h"

void IdealGas::computeForces(System& system)
{
    for(auto& p : system.getParticles())
        p.resetForce();
}