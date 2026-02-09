#pragma once
#include <vector>
#include <cstddef>//en général inclut par vector mais au cas où :) (size_t)
#include "Particle.h"

struct System {
    std::vector<Particle> particles;
    double boxSize;
    size_t targetSize;   // <- nombre de particules attendu

    //on sépare taille actuelle/taille voulue qui évitera des pbs
    System(size_t N, double boxSize_)
        : boxSize(boxSize_), targetSize(N)
    {
        particles.reserve(N);
    }

    size_t size() const {
        return particles.size();
    }

    size_t capacityTarget() const {
        return targetSize;
    }

    //pas nécessairement utiles pour l'instant mais permettent de changer le container, ajouter des vérifications et profiler sans tout casser
    auto& getParticles() {
        return particles;
    }

    const auto& getParticles() const {
        return particles;
    }
};
