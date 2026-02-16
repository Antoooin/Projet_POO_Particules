#include <iostream>
#include <fstream>
#include <memory>

#include "System.h"
#include "Generator.h"
#include "Simulation.h"
#include "VelocityVerlet.h"
#include "IdealGas.h"
#include "LennardJones.h"
#include "Energy.h"

int main()
{
    const size_t N = 200;
    const double boxSize = 20.0;
    const double dt = 0.001;

    // --- 1. Création système ---
    System system(N, boxSize);

    // Génération gaz
    Generator::randomGas(system, 1.0);

    // --- 2. Simulation ---
    // auto potential = std::make_unique<IdealGas>();
    auto potential = std::make_unique<LennardJones>(1.0,1.0,2.5);
    auto integrator = std::make_unique<VelocityVerlet>();

    Simulation sim(
        std::move(system),
        std::move(potential),
        std::move(integrator),
        dt
    );

    // --- 3. Fichier de sortie CSV ---
    std::ofstream out("positions.csv");
    if (!out.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir positions.csv\n";
        return 1;
    }

    // --- 4. Boucle temporelle ---
    std::cout << "Running simulation...\n";
    const size_t steps = 1000;

    for(size_t i = 0; i < steps; ++i)
    {
        sim.step();

        auto& sys = sim.getSystem();
        const auto& particles = sys.getParticles();

        // Export positions de toutes les particules dans le CSV
        for(size_t i = 0; i < particles.size(); ++i) {
            out << particles[i].position.x << "," << particles[i].position.y;
            if(i != particles.size() - 1)
             out << ",";
        }
        out << "\n";


        // Debug console toutes les 100 steps
        if(i % 100 == 0)
        {
            const auto& p0 = particles[0];
            double Ek = Energy::kinetic(sys);

            std::cout << "Step " << i
                      << " | p0=(" << p0.position.x << ", " << p0.position.y << ")"
                      << " | Ek=" << Ek
                      << "\n";
        }
    }

    out.close();
    std::cout << "Simulation finished. Positions saved to positions.csv\n";

    return 0;
}
