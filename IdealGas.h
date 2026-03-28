#pragma once
#include "Potential.h"

// Gaz parfait : aucune interaction entre particules.
// computeForces() remet toutes les forces à zéro.
// energy() retourne 0 par héritage de Potential.
class IdealGas : public Potential {
public:
    void computeForces(System& system) override;
};