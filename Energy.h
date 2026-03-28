#pragma once
#include "System.h"

// Fonctions de calcul d'énergie indépendantes du potentiel.
// Note : l'énergie potentielle LJ est aussi disponible via Potential::energy(),
// qui est la méthode recommandée dans main. potentialLJ() est conservée
// pour usage autonome éventuel.
namespace Energy {
    double kinetic    (const System& system);
    double potentialLJ(const System& system, double epsilon, double sigma, double cutoff);
}