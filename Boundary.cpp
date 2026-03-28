#include "Boundary.h"

void Boundary::applyPeriodic(System& system)
{
    auto& particles    = system.getParticles();
    const double L     = system.boxSize;

    for(auto& p : particles)
    {
        if(p.position.x >= L) p.position.x -= L;
        if(p.position.x <  0) p.position.x += L;
        if(p.position.y >= L) p.position.y -= L;
        if(p.position.y <  0) p.position.y += L;
    }
}