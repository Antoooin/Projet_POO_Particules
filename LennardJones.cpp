#include "LennardJones.h"

void LennardJones::computeForces(System& system)
{
    auto& particles = system.getParticles();
    const double L = system.boxSize;
    const double cutoff2 = cutoff * cutoff; // cutoff en unités absolues

    for(auto& p : particles)
        p.resetForce();

    const size_t N = particles.size();

    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = i+1; j < N; ++j)
        {
            Vec2 dr = particles[i].position - particles[j].position;

            if(dr.x >  L/2) dr.x -= L;
            if(dr.x < -L/2) dr.x += L;
            if(dr.y >  L/2) dr.y -= L;
            if(dr.y < -L/2) dr.y += L;

            double r2 = dr.norm2();

            if(r2 < cutoff2)
            {
                double s2  = (sigma * sigma) / r2;
                double s6  = s2 * s2 * s2;
                double s12 = s6 * s6;

                // F = 24ε/r² * (2(σ/r)¹² - (σ/r)⁶)
                double f = 24.0 * epsilon / r2 * (2.0 * s12 - s6);

                Vec2 F = f * dr;

                particles[i].force += F;
                particles[j].force -= F;
            }
        }
    }
}