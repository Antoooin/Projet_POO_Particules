#include "RK4.h"

// Schéma RK4 appliqué aux équations du mouvement (r' = v, v' = F/m) :
//   k1 évalué en (r0, v0)
//   k2 évalué en (r0 + dt/2 * k1_r, v0 + dt/2 * k1_v)
//   k3 évalué en (r0 + dt/2 * k2_r, v0 + dt/2 * k2_v)
//   k4 évalué en (r0 + dt  * k3_r,  v0 + dt  * k3_v)
//   r(t+dt) = r0 + dt/6 * (k1 + 2k2 + 2k3 + k4)
void RK4::step(System& system, Potential& potential, double dt)
{
    auto& particles = system.getParticles();
    const size_t N  = particles.size();

    // Sauvegarde de l'état en t
    std::vector<Vec2> r0(N), v0(N);
    for(size_t i = 0; i < N; ++i) {
        r0[i] = particles[i].position;
        v0[i] = particles[i].velocity;
    }

    // k1
    potential.computeForces(system);
    std::vector<Vec2> k1_r(N), k1_v(N);
    for(size_t i = 0; i < N; ++i) {
        k1_r[i] = v0[i];
        k1_v[i] = particles[i].force / particles[i].mass;
    }

    // k2 : état intermédiaire en t + dt/2
    for(size_t i = 0; i < N; ++i) {
        particles[i].position = r0[i] + 0.5 * dt * k1_r[i];
        particles[i].velocity = v0[i] + 0.5 * dt * k1_v[i];
    }
    potential.computeForces(system);
    std::vector<Vec2> k2_r(N), k2_v(N);
    for(size_t i = 0; i < N; ++i) {
        k2_r[i] = particles[i].velocity;
        k2_v[i] = particles[i].force / particles[i].mass;
    }

    // k3 : autre état intermédiaire en t + dt/2
    for(size_t i = 0; i < N; ++i) {
        particles[i].position = r0[i] + 0.5 * dt * k2_r[i];
        particles[i].velocity = v0[i] + 0.5 * dt * k2_v[i];
    }
    potential.computeForces(system);
    std::vector<Vec2> k3_r(N), k3_v(N);
    for(size_t i = 0; i < N; ++i) {
        k3_r[i] = particles[i].velocity;
        k3_v[i] = particles[i].force / particles[i].mass;
    }

    // k4 : état en t + dt
    for(size_t i = 0; i < N; ++i) {
        particles[i].position = r0[i] + dt * k3_r[i];
        particles[i].velocity = v0[i] + dt * k3_v[i];
    }
    potential.computeForces(system);
    std::vector<Vec2> k4_r(N), k4_v(N);
    for(size_t i = 0; i < N; ++i) {
        k4_r[i] = particles[i].velocity;
        k4_v[i] = particles[i].force / particles[i].mass;
    }

    // Combinaison finale
    for(size_t i = 0; i < N; ++i) {
        particles[i].position = r0[i] + (dt/6.0)*(k1_r[i] + 2*k2_r[i] + 2*k3_r[i] + k4_r[i]);
        particles[i].velocity = v0[i] + (dt/6.0)*(k1_v[i] + 2*k2_v[i] + 2*k3_v[i] + k4_v[i]);
    }
}