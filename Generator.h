#pragma once
#include "System.h"

namespace Generator {
    // Initialise le système avec des positions sur grille et des vitesses aléatoires.
    // La vitesse du centre de masse est annulée pour éviter toute dérive globale.
    void randomGas(System& system, double maxVelocity) noexcept;
}