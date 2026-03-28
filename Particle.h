#pragma once
#include "Vec2.h"

// Particule ponctuelle 2D : position, vitesse, force accumulée, masse.
// force est initialisée à zéro à la construction pour éviter les valeurs indéfinies.
struct Particle {
    Vec2   position;
    Vec2   velocity;
    Vec2   force{};
    double mass = 1.0;

    // Remet la force à zéro avant chaque recalcul
    void resetForce() { force = Vec2{}; }
};