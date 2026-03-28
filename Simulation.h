#pragma once
#include <memory>
#include "System.h"
#include "Potential.h"
#include "Integrator.h"

// Orchestre la boucle de simulation : intégration + conditions aux limites.
// Possède le System, le Potential et l'Integrator via unique_ptr (ownership exclusif).
class Simulation {
public:
    Simulation(System system_,
               std::unique_ptr<Potential>  potential_,
               std::unique_ptr<Integrator> integrator_,
               double dt_);

    void step();               // avance d'un pas de temps
    void run(size_t steps);    // boucle sur plusieurs pas
    System& getSystem();

private:
    System                     system;
    std::unique_ptr<Potential>  potential;
    std::unique_ptr<Integrator> integrator;
    double dt;
};