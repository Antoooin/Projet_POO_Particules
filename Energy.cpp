#include "Energy.h"

double Energy::kinetic(const System& system)
{
    const auto& particles = system.getParticles();

    double E = 0.0;

    for(const auto& p : particles)
    {
        E += 0.5 * p.mass * dot(p.velocity, p.velocity);
    }

    return E;
}

double Energy::potentialLJ(const System& system, double epsilon, double sigma)
{
    const auto& particles = system.getParticles();
    const size_t N = particles.size();

    double Ep = 0.0;

    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = i+1; j < N; ++j)
        {
            Vec2 rij = particles[i].position - particles[j].position;
            
            // Appliquer les conditions périodiques si nécessaire
            // Ici on fait juste "minimal image" pour boîte carrée
            double L = system.boxSize;
            if(rij.x > 0.5*L) rij.x -= L;
            if(rij.x < -0.5*L) rij.x += L;
            if(rij.y > 0.5*L) rij.y -= L;
            if(rij.y < -0.5*L) rij.y += L;

            double r2 = rij.norm2();
            double sr2 = (sigma * sigma) / r2;
            double sr6 = sr2*sr2*sr2;

            Ep += 4.0 * epsilon * (sr6*sr6 - sr6);
        }
    }

    return Ep;
}
