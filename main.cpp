#include <iostream>
#include <fstream>
#include <memory>

#include "System.h"
#include "Generator.h"
#include "Simulation.h"
#include "VelocityVerlet.h"
#include "LennardJones.h"
#include "Energy.h"
#include "RDF.h"

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

    // --- 3. Fichiers de sortie ---
    std::ofstream posFile("positions.csv");
    std::ofstream velFile("velocities.csv");
    std::ofstream energyFile("energies.csv");

    if (!posFile || !velFile || !energyFile) {
        std::cerr << "Erreur ouverture fichiers output\n";
        return 1;
    }

    // Header énergie
    energyFile << "step,Ek,Ep,Etot\n";

    // --- 4. RDF ---
    const double rMax = boxSize * 0.5;
    const int bins = 100;
    RDF rdf(rMax, bins);

    size_t rdfSamples = 0;
    const size_t rdfStart = 2000; // ignore phase transitoire

    // --- 5. Simulation ---
    std::cout << "Running simulation...\n";
    const size_t steps = 10000;

    for(size_t step = 0; step < steps; ++step)
    {
        sim.step();
        auto& sys = sim.getSystem();
        const auto& particles = sys.getParticles();

        // ---------------- POSITIONS ----------------
        for(size_t i = 0; i < particles.size(); ++i) {
            posFile << particles[i].position.x << "," << particles[i].position.y;
            if(i != particles.size() - 1) posFile << ",";
        }
        posFile << "\n";

        // ---------------- VELOCITIES ----------------
        for(size_t i = 0; i < particles.size(); ++i) {
            velFile << particles[i].velocity.x << "," << particles[i].velocity.y;
            if(i != particles.size() - 1) velFile << ",";
        }
        velFile << "\n";

        // ---------------- ENERGIES ----------------
        double Ek = Energy::kinetic(sys);
        double Ep = Energy::potentialLJ(sys, 2.0, 1.0);
        double Etot = Ek + Ep;

        energyFile << step << "," << Ek << "," << Ep << "," << Etot << "\n";

        // Debug console
        if(step % 500 == 0)
        {
            std::cout << "Step " << step
                      << " | Ek=" << Ek
                      << " | Ep=" << Ep
                      << " | Etot=" << Etot
                      << "\n";
        }

        // ---------------- RDF sampling ----------------
        if(step > rdfStart) {
            rdf.sample(sys);
            rdfSamples++;
        }
    }

    posFile.close();
    velFile.close();
    energyFile.close();

    // --- 6. Finalisation RDF ---
    std::cout << "Normalizing RDF...\n";
    rdf.normalize(sim.getSystem(), rdfSamples);
    rdf.saveCSV("rdf.csv");

    std::cout << "Simulation finished.\n";
    std::cout << "Saved: positions.csv, velocities.csv, energies.csv, rdf.csv\n";

    return 0;
}
