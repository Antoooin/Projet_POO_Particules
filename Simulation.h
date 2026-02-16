#pragma once
#include <memory>
#include "System.h"
#include "Potential.h"
#include "Integrator.h"

class Simulation {
public:
    Simulation(System system_,
               std::unique_ptr<Potential> potential_,
               std::unique_ptr<Integrator> integrator_,
               double dt_);

    void step();
    void run(size_t steps);

    System& getSystem();

private:
    System system;
    std::unique_ptr<Potential> potential;
    std::unique_ptr<Integrator> integrator;
    double dt;
};
