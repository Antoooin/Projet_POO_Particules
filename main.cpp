#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <cmath>

#include "Boundary.h"
#include "Config.h"
#include "Energy.h"
#include "Euler.h"
#include "Generator.h"
#include "IdealGas.h"
#include "Integrator.h"
#include "LennardJones.h"
#include "Particle.h"
#include "Potential.h"
#include "RDF.h"
#include "RK4.h"
#include "Simulation.h"
#include "System.h"
#include "VelocityVerlet.h"

int main()
{
    // --- 1. Chargement de la configuration ---
    Config cfg = loadConfig("config.txt");
    auto start = std::chrono::high_resolution_clock::now();

    // --- 2. Initialisation du système ---
    System system(cfg.N, cfg.boxSize);
    Generator::randomGas(system, cfg.initVelocity);

    // --- 3. Sélection du potentiel ---
    std::unique_ptr<Potential> potential;

    if(cfg.potential == "LJ")
    {
        std::cout << "Using Lennard-Jones potential\n";
        potential = std::make_unique<LennardJones>(cfg.epsilon, cfg.sigma, cfg.cutoff);
    }
    else if(cfg.potential == "NONE")
    {
        std::cout << "Using Ideal Gas (no interactions)\n";
        potential = std::make_unique<IdealGas>();
    }
    else
    {
        std::cerr << "Unknown potential type: '" << cfg.potential << "'\n";
        return 1;
    }

    // --- 4. Sélection de l'intégrateur ---
    std::unique_ptr<Integrator> integrator;

    if(cfg.integrator == "VV")
    {
        std::cout << "Integrator: Velocity Verlet\n";
        integrator = std::make_unique<VelocityVerlet>();
    }
    else if(cfg.integrator == "Euler")
    {
        std::cout << "Integrator: Euler\n";
        integrator = std::make_unique<Euler>();
    }
    else if(cfg.integrator == "RK4")
    {
        std::cout << "Integrator: RK4\n";
        integrator = std::make_unique<RK4>();
    }
    else
    {
        std::cerr << "Unknown integrator: '" << cfg.integrator << "'\n";
        return 1;
    }

    // --- 5. Construction de la simulation ---
    Simulation sim(std::move(system), std::move(potential), std::move(integrator), cfg.dt);

    // --- 6. Fichiers de sortie ---
    std::ofstream posFile("positions.csv");
    std::ofstream velFile("velocities.csv");
    std::ofstream energyFile("energies.csv");

    if(!posFile || !velFile || !energyFile) {
        std::cerr << "Erreur ouverture fichiers output\n";
        return 1;
    }
    energyFile << "step,Ek,Ep,Etot,dE_rel\n";

    // --- 7. RDF ---
    RDF    rdf(cfg.boxSize * 0.5, cfg.rdfBins);
    size_t rdfSamples = 0;

    // --- 8. Boucle principale ---
    std::cout << "Running simulation...\n";
    double E0 = 0.0;  // énergie au step 0, pour la dérive relative
    for(size_t step = 0; step < cfg.steps; ++step)
    {
        sim.step();
        auto& sys             = sim.getSystem();
        const auto& particles = sys.getParticles();

        // Positions (format : x0,y0,x1,y1,...,xN,yN)
        for(size_t i = 0; i < particles.size(); ++i) {
            posFile << particles[i].position.x << "," << particles[i].position.y;
            if(i != particles.size() - 1) posFile << ",";
        }
        posFile << "\n";

        // Vitesses (même format)
        for(size_t i = 0; i < particles.size(); ++i) {
            velFile << particles[i].velocity.x << "," << particles[i].velocity.y;
            if(i != particles.size() - 1) velFile << ",";
        }
        velFile << "\n";

        // Énergies : Ep via le polymorphisme (IdealGas retourne 0 automatiquement)
        double Ek   = Energy::kinetic(sys);
        double Ep = sim.potentialEnergy();
        double Etot = Ek + Ep;

        if(step == 0) E0 = Etot;
        double dE_rel = (E0 != 0.0) ? (Etot - E0) / std::abs(E0) : 0.0;

        energyFile << step << "," << Ek << "," << Ep << "," << Etot << "," << dE_rel << "\n";

        // Visualisation de la simulation en cours (outil de débug)
        if(step % 500 == 0)
            std::cout << "Step " << step
                      << " | Ek=" << Ek << " | Ep=" << Ep
                      << " | Etot=" << Etot << " | dE_rel=" << dE_rel << "\n";

        // Échantillonnage RDF après thermalisation
        if(step > cfg.rdfStart) {
            rdf.sample(sys);
            ++rdfSamples;
        }
    }

    posFile.close();
    velFile.close();
    energyFile.close();

    // --- 9. Normalisation et sauvegarde de la RDF ---
    std::cout << "Normalizing RDF...\n";
    rdf.normalize(sim.getSystem(), rdfSamples);
    rdf.saveCSV("rdf.csv");

    auto end     = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();
    std::cout << "Simulation finished in " << elapsed << " seconds\n";
    std::cout << "Saved: positions.csv, velocities.csv, energies.csv, rdf.csv\n";

    return 0;
}