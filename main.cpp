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
    const size_t N = 500;
    const double boxSize = 20.0;
    const double dt = 0.0005;

    // --- 1. Création système ---
    System system(N, boxSize);

    // Génération gaz
    Generator::randomGas(system, 0.2);

    // --- 2. Simulation ---
    auto potential = std::make_unique<LennardJones>(2.0, 1.0, 2.5);
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
    const size_t steps = 10000;

    for(size_t step = 0; step < steps; ++step)
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
        if(step % 100 == 0)
        {
            double Ek = Energy::kinetic(sys);
            double Ep = Energy::potentialLJ(sys, 1.0, 1.0); // epsilon,sigma = 1.0
            double Etot = Ek + Ep;

            std::cout << "Step " << step
                      << " | Ek=" << Ek
                      << " | Ep=" << Ep
                      << " | Etot=" << Etot
                      << "\n";
        }
    }

    out.close();
    std::cout << "Simulation finished. Positions saved to positions.csv\n";

    return 0;
}
