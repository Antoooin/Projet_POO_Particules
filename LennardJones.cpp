#include "LennardJones.h"

void LennardJones::computeForces(System& system)
{
    auto& particles = system.getParticles();
    const double L = system.boxSize;
    const double cutoff2 = cutoff * sigma * cutoff * sigma; // carré du cutoff

    // Reset forces
    for(auto& p : particles)
        p.resetForce();

    const size_t N = particles.size();

    // Boucle sur toutes les paires
    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = i+1; j < N; ++j)
        {
            // Distance minimale avec conditions périodiques
            Vec2 dr = particles[i].position - particles[j].position;

            // PBC : "minimum image convention"
            if(dr.x >  L/2) dr.x -= L;
            if(dr.x < -L/2) dr.x += L;
            if(dr.y >  L/2) dr.y -= L;
            if(dr.y < -L/2) dr.y += L;

            double r2 = dr.norm2();

            if(r2 < cutoff2)
            {
                double r2inv = 1.0 / r2;
                double r6inv = r2inv * r2inv * r2inv;
                double f = 24.0 * epsilon * r2inv * r6inv * (2.0 * std::pow(sigma,12) * r6inv - std::pow(sigma,6));

                Vec2 F = f * dr;

                particles[i].force += F;
                particles[j].force -= F;
            }
        }
    }
}
