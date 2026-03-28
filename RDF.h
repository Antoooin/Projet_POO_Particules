#pragma once
#include <vector>
#include <string>
#include "System.h"

// Fonction de distribution radiale g(r) en 2D.
// Mesure la probabilité de trouver une particule à distance r d'une particule de référence,
// normalisée par la densité d'un gaz parfait homogène.
class RDF {
public:
    RDF(double rMax, int bins);

    // Accumule un instantané des distances inter-particules dans l'histogramme
    void sample(const System& system);

    // Normalise l'histogramme par le nombre d'échantillons et la densité idéale
    void normalize(const System& system, int nSamples);

    void saveCSV(const std::string& filename) const;

private:
    double rMax;
    double dr;
    int    bins;

    std::vector<double> histogram;
    std::vector<double> radii;     // centres des bins
};