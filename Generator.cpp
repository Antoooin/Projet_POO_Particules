#include "Generator.h"

#include <cmath>
#include <random>

/*
 Génère un gaz "propre" :

 - positions sur une grille régulière pour éviter les overlaps
   (sinon Lennard-Jones explose → forces infinies)

 - petit bruit aléatoire pour casser la symétrie parfaite
   (une grille parfaite peut créer des artefacts physiques)

 - vitesses aléatoires

 - on impose que la vitesse du centre de masse = 0
   pour éviter que tout le gaz dérive dans la boîte

 IMPORTANT :
 On ne dépend JAMAIS de vector.capacity().
 On utilise le nombre cible défini dans System.
*/

void Generator::randomGas(System& system, double maxVelocity) noexcept
{
    auto& particles = system.getParticles();

    // Reset propre si on régénère un système
    particles.clear();
    particles.reserve(system.capacityTarget());

    const size_t N = system.capacityTarget();
    const double L = system.boxSize;

    // 1) Construction de la grille

    /*
     On veut au moins N cellules.

     sqrt(N) → nombre de cellules par dimension
     ceil → garantit qu'on a assez de place
    */
    const size_t n = static_cast<size_t>(std::ceil(std::sqrt(N)));
    const double spacing = L / static_cast<double>(n);

    // 2) Générateurs aléatoires

    std::mt19937 rng(std::random_device{}());

    // Petit bruit (10% du pas)
    std::uniform_real_distribution<double> noise(
        -0.1 * spacing, 0.1 * spacing
    );

    std::uniform_real_distribution<double> vel(
        -maxVelocity, maxVelocity
    );

    // Servira pour annuler la vitesse du CDM
    Vec2 vcm{};

    // 3) Création des particules

    /*
      On parcourt les particules de 0 → N
      puis on reconstruit les coordonnées de grille :

          i = k / n
          j = k % n
    */

    for(size_t k = 0; k < N; ++k)
    {
        size_t i = k / n;
        size_t j = k % n;

        Vec2 position{
            (i + 0.5) * spacing + noise(rng),
            (j + 0.5) * spacing + noise(rng)
        };

        Vec2 velocity{
            vel(rng),
            vel(rng)
        };

        vcm += velocity;

        // Construction directe dans le vector (zéro copie)
        particles.emplace_back(
            Particle{
                position,
                velocity
            }
        );
    }

    // 4) Suppression vitesse CDM


    vcm = (1.0 / static_cast<double>(N)) * vcm;

    for(auto& p : particles)
        p.velocity -= vcm;
}
