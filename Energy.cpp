#include "Energy.h"

double Energy::kinetic(const System& system)
{
    const auto& particles = system.getParticles();
    double E = 0.0;

    for(const auto& p : particles)
        E += 0.5 * p.mass * dot(p.velocity, p.velocity);

    return E;
}

double Energy::potentialLJ(const System& system,
                           double epsilon,
                           double sigma,
                           double cutoff)
{
    const auto& particles = system.getParticles();
    const size_t N = particles.size();
    const double L = system.boxSize;
    const double cutoff2 = cutoff * cutoff;

    double Ep = 0.0;

    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = i+1; j < N; ++j)
        {
            Vec2 rij = particles[i].position - particles[j].position;

            if(rij.x >  0.5*L) rij.x -= L;
            if(rij.x < -0.5*L) rij.x += L;
            if(rij.y >  0.5*L) rij.y -= L;
            if(rij.y < -0.5*L) rij.y += L;

            double r2 = rij.norm2();

            if(r2 >= cutoff2) continue;

            double s2  = (sigma * sigma) / r2;
            double s6  = s2 * s2 * s2;
            double s12 = s6 * s6;

            Ep += 4.0 * epsilon * (s12 - s6);
        }
    }

    return Ep;
}