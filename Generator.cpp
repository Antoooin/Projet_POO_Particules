#include "Generator.h"
#include <cmath>
#include <random>

// Initialisation "propre" du gaz :
//  - grille régulière pour éviter les overlaps (forces LJ infinies à r→0)
//  - bruit aléatoire de ±10% du pas pour casser la symétrie cristalline
//  - vitesses uniformément distribuées dans [-maxVelocity, maxVelocity]
//  - vitesse du centre de masse annulée (pas de dérive globale de la boîte)
void Generator::randomGas(System& system, double maxVelocity) noexcept
{
    auto& particles = system.getParticles();
    particles.clear();
    particles.reserve(system.capacityTarget());

    const size_t N       = system.capacityTarget();
    const double L       = system.boxSize;
    const size_t n       = static_cast<size_t>(std::ceil(std::sqrt(N)));
    const double spacing = L / static_cast<double>(n);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> noise(-0.1 * spacing, 0.1 * spacing);
    std::uniform_real_distribution<double> vel(-maxVelocity, maxVelocity);

    Vec2 vcm{};

    // Placement sur grille 2D : indice k → (ligne i, colonne j)
    for(size_t k = 0; k < N; ++k)
    {
        size_t i = k / n;
        size_t j = k % n;

        Vec2 position{ (i + 0.5) * spacing + noise(rng),
                       (j + 0.5) * spacing + noise(rng) };
        Vec2 velocity{ vel(rng), vel(rng) };

        vcm += velocity;
        particles.emplace_back(Particle{ position, velocity });
    }

    // Soustraction de la vitesse du centre de masse
    vcm = (1.0 / static_cast<double>(N)) * vcm;
    for(auto& p : particles) p.velocity -= vcm;
}