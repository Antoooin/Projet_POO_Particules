#include "IdealGas.h"

/*
  Gaz parfait :
  Aucune interaction → forces nulles
*/

void IdealGas::computeForces(System& system)
{
    auto& particles = system.getParticles();

    for(auto& p : particles)
        p.resetForce();
}
