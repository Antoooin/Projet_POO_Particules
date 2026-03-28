#include "Euler.h"

// Schéma d'Euler explicite :
//   r(t+dt) = r(t) + dt * v(t)
//   v(t+dt) = v(t) + dt * a(t)
// Erreur locale en O(dt²), erreur globale en O(dt).
void Euler::step(System& system, Potential& potential, double dt)
{
    auto& particles = system.getParticles();
    potential.computeForces(system);

    for(auto& p : particles)
    {
        Vec2 a    = p.force / p.mass;
        p.position += dt * p.velocity;
        p.velocity += dt * a;
    }
}