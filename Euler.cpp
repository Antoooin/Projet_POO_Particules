#include "Euler.h"

void Euler::step(System& system,
                 Potential& potential,
                 double dt)
{
    auto& particles = system.getParticles();

    // Calcul des forces actuelles
    potential.computeForces(system);

    for(auto& p : particles)
    {
        Vec2 a = p.force / p.mass;

        // Euler explicite
        p.position += dt * p.velocity;
        p.velocity += dt * a;
    }
}