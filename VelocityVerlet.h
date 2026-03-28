#pragma once
#include "Integrator.h"

// Intégrateur Velocity Verlet, ordre 2, symplectique.
// Conserve une énergie "fantôme" proche de l'énergie réelle sur des temps arbitrairement longs.
class VelocityVerlet : public Integrator {
public:
    void step(System& system, Potential& potential, double dt) override;
};