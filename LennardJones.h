#pragma once
#include "Potential.h"
#include <cmath>

class LennardJones : public Potential {
public:
    LennardJones(double epsilon_ = 1.0, double sigma_ = 1.0, double cutoff_ = 2.5)
        : epsilon(epsilon_), sigma(sigma_), cutoff(cutoff_) {}

    void computeForces(System& system) override;

private:
    double epsilon;
    double sigma;
    double cutoff; // en unités de sigma
};
