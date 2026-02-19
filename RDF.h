#pragma once
#include <vector>
#include <string>
#include "System.h"

class RDF {
public:
    RDF(double rMax, int bins);

    void sample(const System& system);
    void normalize(const System& system, int nSamples);

    void saveCSV(const std::string& filename) const;

private:
    double rMax;
    double dr;
    int bins;

    std::vector<double> histogram;
    std::vector<double> radii;
};
