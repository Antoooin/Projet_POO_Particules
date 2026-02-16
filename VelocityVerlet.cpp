#include "VelocityVerlet.h"
#include "Boundary.h"
#include "Potential.h"

void VelocityVerlet::step(System& system, Potential& potential, double dt)
{
    auto& particles = system.getParticles();

    // --- Half velocity + position ---
    for(auto& p : particles)
    {
        Vec2 a = (1.0 / p.mass) * p.force;
        p.velocity += 0.5 * dt * a;
        p.position += dt * p.velocity;
    }

    // --- PBC ---
    Boundary::applyPeriodic(system);

    // --- Reset forces ---
    for(auto& p : particles)
        p.resetForce();

    // --- Recompute forces ---
    potential.computeForces(system);

    // --- Second half velocity ---
    for(auto& p : particles)
    {
        Vec2 a = (1.0 / p.mass) * p.force;
        p.velocity += 0.5 * dt * a;
    }
}
