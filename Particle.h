#pragma once
#include "Vec2.h"

struct Particle{
    Vec2 position;
    Vec2 velocity;
    //On initialise Particle.force à 0,0 pour s'éviter des bugs
    Vec2 force{};
    double mass={1.0};

    void resetForce(){
        force=Vec2{};
    }
};