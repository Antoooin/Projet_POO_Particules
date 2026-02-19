#include "RDF.h"
#include <cmath>
#include <fstream>

constexpr double PI = 3.14159265358979323846;

RDF::RDF(double rMax_, int bins_)
    : rMax(rMax_), bins(bins_)
{
    dr = rMax / bins;

    histogram.assign(bins, 0.0);
    radii.resize(bins);

    for(int i = 0; i < bins; ++i)
        radii[i] = (i + 0.5) * dr;
}

void RDF::sample(const System& system)
{
    const auto& particles = system.getParticles();
    const double L = system.boxSize;
    const size_t N = particles.size();

    for(size_t i = 0; i < N; ++i)
    {
        for(size_t j = i + 1; j < N; ++j)
        {
            Vec2 rij = particles[j].position - particles[i].position;

            // --- minimum image convention (PBC) ---
            if(rij.x >  0.5 * L) rij.x -= L;
            if(rij.x < -0.5 * L) rij.x += L;
            if(rij.y >  0.5 * L) rij.y -= L;
            if(rij.y < -0.5 * L) rij.y += L;

            double r = rij.norm();
            if(r < rMax)
            {
                int bin = static_cast<int>(r / dr);
                histogram[bin] += 2.0; // i-j et j-i
            }
        }
    }
}

void RDF::normalize(const System& system, int nSamples)
{
    const size_t N = system.getParticles().size();
    const double V = system.boxSize * system.boxSize;
    const double density = N / V;

    for(int i = 0; i < bins; ++i)
    {
        double r = radii[i];

        // aire anneau en 2D
        double shellArea = 2.0 * PI * r * dr;

        double ideal = density * shellArea * N;

        histogram[i] /= (ideal * nSamples);
    }
}

void RDF::saveCSV(const std::string& filename) const
{
    std::ofstream out(filename);
    for(int i = 0; i < bins; ++i)
        out << radii[i] << "," << histogram[i] << "\n";
}
