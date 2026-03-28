#include "VelocityVerlet.h"

// Schéma Velocity Verlet :
//   1. demi-kick  : v += ½ dt * a(t)
//   2. drift      : r += dt * v
//   3. new forces : a(t+dt) = F(r(t+dt)) / m
//   4. demi-kick  : v += ½ dt * a(t+dt)
// Erreur globale en O(dt²), mais symplectique → pas de dérive séculaire de l'énergie.
void VelocityVerlet::step(System& system, Potential& potential, double dt)
{
    auto& particles = system.getParticles();

    for(auto& p : particles) {
        p.velocity += 0.5 * dt * (p.force / p.mass);  // demi-kick
    }
    for(auto& p : particles) {
        p.position += dt * p.velocity;                 // drift
    }

    potential.computeForces(system);                   // nouvelles forces

    for(auto& p : particles) {
        p.velocity += 0.5 * dt * (p.force / p.mass);  // demi-kick final
    }
}