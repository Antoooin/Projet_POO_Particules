#pragma once
#include "System.h"

namespace Energy {

double kinetic(const System& system);

double potentialLJ(const System& system,
                   double epsilon,
                   double sigma,
                   double cutoff);
}