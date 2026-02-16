#include <iostream>
#include <memory>

#include "System.h"
#include "Generator.h"
#include "Simulation.h"
#include "VelocityVerlet.h"
#include "IdealGas.h"
#include "Energy.h"


int main()
{
    const size_t N = 100;
    const double boxSize = 20.0;
    const double dt = 0.01;

    // --- 1. Création système ---
    System system(N, boxSize);

    // Génération gaz
    Generator::randomGas(system, 1.0);

    // --- 2. Simulation ---
    auto potential = std::make_unique<IdealGas>();
    auto integrator = std::make_unique<VelocityVerlet>();

    Simulation sim(
        std::move(system),
        std::move(potential),
        std::move(integrator),
        dt
    );

    // --- 3. Test évolution ---
    std::cout << "Running simulation...\n";

    const size_t steps = 1000;

    for(size_t i = 0; i < steps; ++i)
    {
        sim.step();

        // Debug toutes les 100 steps
        if(i % 100 == 0)
        {
        auto& sys = sim.getSystem();
        const auto& p = sys.getParticles()[0];

        double Ek = Energy::kinetic(sys);

        std::cout << "Step " << i
                << " | p0=(" << p.position.x << ", " << p.position.y << ")"
                << " | Ek=" << Ek
                << "\n";
}
    }

    std::cout << "Simulation finished.\n";
}
