#include "Boundary.h"

/*
  Conditions périodiques simples :
  Si une particule sort de la boîte, elle rentre de l'autre côté.
*/

void Boundary::applyPeriodic(System& system)
{
    auto& particles = system.getParticles();
    const double L = system.boxSize;

    for(auto& p : particles)
    {
        // X
        if(p.position.x >= L) p.position.x -= L;
        if(p.position.x <  0) p.position.x += L;

        // Y
        if(p.position.y >= L) p.position.y -= L;
        if(p.position.y <  0) p.position.y += L;
    }
}
