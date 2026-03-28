#pragma once
#include "System.h"
#include "Potential.h"

// Interface abstraite pour les algorithmes d'intégration temporelle.
// Chaque sous-classe implémente un schéma différent (Euler, VV, RK4).
class Integrator {
public:
    virtual ~Integrator() = default;

    // Avance le système d'un pas de temps dt
    virtual void step(System& system, Potential& potential, double dt) = 0;
};