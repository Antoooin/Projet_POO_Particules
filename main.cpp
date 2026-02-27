#include <iostream>
#include <fstream>
#include <memory>

#include "Boundary.h"
#include "Config.h"
#include "Energy.h"
#include "Generator.h"
#include "IdealGas.h"
#include "Integrator.h"
#include "LennardJones.h"
#include "Particle.h"
#include "Potential.h"
#include "RDF.h"
#include "Simulation.h"
#include "System.h"
#include "VelocityVerlet.h"

int main()
{
    // -------- LOAD CONFIG --------
    Config cfg = loadConfig("config.txt");

    // --- 1. Création système ---
    System system(cfg.N, cfg.boxSize);

    // Génération gaz
    Generator::randomGas(system, cfg.initVelocity);

    // --- 2. Choix Potentiel ---
    std::unique_ptr<Potential> potential;

    if(cfg.potential == "LJ")
    {
        std::cout << "Using Lennard-Jones potential\n";

        potential = std::make_unique<LennardJones>(
            cfg.epsilon,
            cfg.sigma,
            cfg.cutoff
        );
    }
    else if(cfg.potential == "NONE")
    {
        std::cout << "Using Ideal Gas (no interactions)\n";
        potential = std::make_unique<IdealGas>();
    }
    else
    {
        std::cerr << "Unknown potential type\n";
        return 1;
    }


    auto integrator = std::make_unique<VelocityVerlet>();

    Simulation sim(
        std::move(system),
        std::move(potential),
        std::move(integrator),
        cfg.dt
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
    const double rMax = cfg.boxSize * 0.5;
    RDF rdf(rMax, cfg.rdfBins);

    size_t rdfSamples = 0;

    // --- 5. Simulation ---
    std::cout << "Running simulation...\n";
    const size_t steps = 10000;

    for(size_t step = 0; step < cfg.steps; ++step)
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
        double Ep = Energy::potentialLJ(sys, cfg.epsilon, cfg.sigma);
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
        if(step > cfg.rdfStart) {
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
