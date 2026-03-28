#include "LennardJones.h"

// Calcule les forces de Lennard-Jones sur toutes les paires (i,j), i<j.
// Utilise la convention "minimum image" pour les conditions aux limites périodiques.
// La force est dérivée analytiquement : F = -dV/dr
//   f = (24ε/r²) [ 2(σ/r)¹² - (σ/r)⁶ ]
// Les interactions au-delà de cutoff sont ignorées (complexité O(N²) réduite en pratique).
void LennardJones::computeForces(System& system)
{
    auto& particles = system.getParticles();
    const double L       = system.boxSize;
    const double cutoff2 = cutoff * cutoff;

    for(auto& p : particles) p.resetForce();

    const size_t N = particles.size();
    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = i+1; j < N; ++j)
        {
            Vec2 dr = particles[i].position - particles[j].position;

            // Minimum image convention (boîte carrée)
            if(dr.x >  L/2) dr.x -= L;
            if(dr.x < -L/2) dr.x += L;
            if(dr.y >  L/2) dr.y -= L;
            if(dr.y < -L/2) dr.y += L;

            double r2 = dr.norm2();
            if(r2 >= cutoff2) continue;

            // Calcul sans std::pow pour la performance
            double s2  = (sigma * sigma) / r2;
            double s6  = s2 * s2 * s2;
            double s12 = s6 * s6;

            double f  = 24.0 * epsilon / r2 * (2.0 * s12 - s6);
            Vec2   F  = f * dr;

            particles[i].force += F;   // 3ème loi de Newton
            particles[j].force -= F;
        }
    }
}

// Énergie potentielle totale : somme sur toutes les paires non équivalentes.
// Cohérente avec computeForces() : même cutoff, même convention PBC.
double LennardJones::energy(const System& system) const
{
    const auto& particles = system.getParticles();
    const size_t N        = particles.size();
    const double L        = system.boxSize;
    const double cutoff2  = cutoff * cutoff;

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