#pragma once
#include "System.h"

// Interface abstraite pour les potentiels d'interaction.
// Toute nouvelle interaction doit hériter de cette classe et implémenter
// computeForces(). energy() a une implémentation par défaut à 0
// (utilisée par IdealGas, qui n'a pas d'énergie potentielle).
class Potential {
public:
    virtual ~Potential() = default;

    // Calcule et accumule les forces sur toutes les particules
    virtual void computeForces(System& system) = 0;

    // Retourne l'énergie potentielle totale du système
    virtual double energy(const System& system) const { return 0.0; }
};