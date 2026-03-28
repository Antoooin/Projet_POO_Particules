#include "RDF.h"
#include <cmath>
#include <fstream>

constexpr double PI = 3.14159265358979323846;

RDF::RDF(double rMax_, int bins_)
    : rMax(rMax_), bins(bins_), dr(rMax_ / bins_)
{
    histogram.assign(bins, 0.0);
    radii.resize(bins);
    for(int i = 0; i < bins; ++i)
        radii[i] = (i + 0.5) * dr;  // centre du bin i
}

// Parcourt toutes les paires (i,j), applique la convention de l'image minimale,
// et incrémente de 2 le bin correspondant (compte i→j et j→i).
void RDF::sample(const System& system)
{
    const auto& particles = system.getParticles();
    const double L        = system.boxSize;
    const size_t N        = particles.size();

    for(size_t i = 0; i < N; ++i)
        for(size_t j = i + 1; j < N; ++j)
        {
            Vec2 rij = particles[j].position - particles[i].position;

            if(rij.x >  0.5*L) rij.x -= L;
            if(rij.x < -0.5*L) rij.x += L;
            if(rij.y >  0.5*L) rij.y -= L;
            if(rij.y < -0.5*L) rij.y += L;

            double r = rij.norm();
            if(r < rMax)
                histogram[static_cast<int>(r / dr)] += 2.0;
        }
}

// Normalisation 2D : g(r) = histogramme / (N * ρ * aire_anneau * nSamples)
// où aire_anneau = 2π r dr  (anneau en 2D)
void RDF::normalize(const System& system, int nSamples)
{
    const size_t N   = system.getParticles().size();
    const double V   = system.boxSize * system.boxSize;
    const double rho = static_cast<double>(N) / V;  // densité surfacique

    for(int i = 0; i < bins; ++i)
    {
        double shellArea = 2.0 * PI * radii[i] * dr;
        histogram[i] /= (rho * shellArea * N * nSamples);
    }
}

void RDF::saveCSV(const std::string& filename) const
{
    std::ofstream out(filename);
    for(int i = 0; i < bins; ++i)
        out << radii[i] << "," << histogram[i] << "\n";
}