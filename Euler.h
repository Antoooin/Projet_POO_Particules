#pragma once
#include "Integrator.h"

// Intégrateur d'Euler explicite, ordre 1.
// Simple mais non-symplectique : l'énergie dérive systématiquement.
// Déconseillé pour les potentiels raides comme Lennard-Jones.
class Euler : public Integrator {
public:
    void step(System& system, Potential& potential, double dt) override;
};