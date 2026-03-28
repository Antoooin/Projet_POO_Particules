#pragma once
#include "Integrator.h"

// Intégrateur Runge-Kutta d'ordre 4.
// Très précis (erreur globale en O(dt⁴)) mais non-symplectique :
// dérive lente de l'énergie sur des temps longs.
// Appelle computeForces() 4 fois par step → ~4× plus lent que VV ou Euler.
class RK4 : public Integrator {
public:
    void step(System& system, Potential& potential, double dt) override;
};