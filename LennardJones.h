#pragma once
#include "Potential.h"
#include <cmath>

// Potentiel de Lennard-Jones tronqué :
//   V(r) = 4ε [ (σ/r)¹² - (σ/r)⁶ ]   pour r < cutoff
//   V(r) = 0                            pour r ≥ cutoff
class LennardJones : public Potential {
public:
    // cutoff en unités absolues (mêmes que σ)
    LennardJones(double epsilon_ = 1.0, double sigma_ = 1.0, double cutoff_ = 2.5)
        : epsilon(epsilon_), sigma(sigma_), cutoff(cutoff_) {}

    void   computeForces(System& system)        override;
    double energy(const System& system)   const override;

private:
    double epsilon;  // profondeur du puits
    double sigma;    // distance à laquelle V = 0
    double cutoff;   // rayon de troncature en unités absolues
};