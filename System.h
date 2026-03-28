#pragma once
#include <vector>
#include <cstddef>
#include "Particle.h"

// Contient l'ensemble des particules et la géométrie de la boîte carrée.
// targetSize sépare le nombre de particules voulu du nombre réellement créé,
// ce qui évite de dépendre de vector::capacity().
struct System {
    std::vector<Particle> particles;
    double boxSize;
    size_t targetSize;

    System(size_t N, double boxSize_)
        : boxSize(boxSize_), targetSize(N)
    {
        particles.reserve(N);
    }

    size_t size()            const { return particles.size(); }
    size_t capacityTarget()  const { return targetSize; }

    // Accesseurs non-const et const : permettent de changer le conteneur
    // ou d'ajouter des vérifications sans modifier le reste du code.
    auto&       getParticles()       { return particles; }
    const auto& getParticles() const { return particles; }
};