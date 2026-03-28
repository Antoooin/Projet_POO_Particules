#include "Simulation.h"
#include "Boundary.h"

Simulation::Simulation(System system_,
                       std::unique_ptr<Potential>  potential_,
                       std::unique_ptr<Integrator> integrator_,
                       double dt_)
    : system    (std::move(system_)),
      potential (std::move(potential_)),
      integrator(std::move(integrator_)),
      dt(dt_)
{
    // Forces initiales nécessaires pour le premier demi-kick de VelocityVerlet
    potential->computeForces(system);
}

void Simulation::step()
{
    integrator->step(system, *potential, dt);
    Boundary::applyPeriodic(system);
}

void Simulation::run(size_t steps)
{
    for(size_t i = 0; i < steps; ++i) step();
}

System& Simulation::getSystem() { return system; }