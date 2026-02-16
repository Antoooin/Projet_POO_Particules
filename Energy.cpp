#include "Energy.h"

double Energy::kinetic(const System& system)
{
    const auto& particles = system.getParticles();

    double E = 0.0;

    for(const auto& p : particles)
    {
        E += 0.5 * p.mass * dot(p.velocity, p.velocity);
    }

    return E;
}
