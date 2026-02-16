#include "VelocityVerlet.h"

void VelocityVerlet::step(System& system,
                          Potential& potential,
                          double dt)
{
    auto& particles = system.getParticles();

    // --- 1. demi kick ---
    for(auto& p : particles)
    {
        Vec2 a = p.force / p.mass;
        p.velocity += 0.5 * dt * a;
    }

    // --- 2. drift ---
    for(auto& p : particles)
    {
        p.position += dt * p.velocity;
    }

    // --- 3. nouvelles forces ---
    potential.computeForces(system);

    // --- 4. demi kick final ---
    for(auto& p : particles)
    {
        Vec2 a = p.force / p.mass;
        p.velocity += 0.5 * dt * a;
    }
}
