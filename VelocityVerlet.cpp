#include "VelocityVerlet.h"

/*
  Velocity Verlet scheme

  Hypothèse :
  - Les forces F(t) sont déjà calculées avant l'appel.
  - Après mise à jour des positions, il faut recalculer les forces
    AVANT la seconde moitié de mise à jour des vitesses.
*/

void VelocityVerlet::step(System& system, double dt)
{
    auto& particles = system.getParticles();

    // 1) v(t + dt/2) et r(t + dt)
    for(auto& p : particles)
    {
        Vec2 acceleration = (1.0 / p.mass) * p.force;

        p.velocity += 0.5 * dt * acceleration;
        p.position += dt * p.velocity;
    }

    // ⚠️ IMPORTANT :
    // Ici normalement on doit recalculer les forces :
    //
    // potential.computeForces(system);
    //
    // MAIS pour l'instant (gaz parfait) les forces sont nulles,
    // donc on saute cette étape.
    //
    // Quand tu ajouteras LJ, il faudra l'appeler ici.

    // 2) v(t + dt)
    for(auto& p : particles)
    {
        Vec2 acceleration = (1.0 / p.mass) * p.force;
        p.velocity += 0.5 * dt * acceleration;
    }
}
