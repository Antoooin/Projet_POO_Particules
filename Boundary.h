#pragma once
#include "System.h"

namespace Boundary {
    // Applique les conditions aux limites périodiques (boîte carrée de côté boxSize).
    // Une particule sortant d'un côté rentre du côté opposé.
    void applyPeriodic(System& system);
}