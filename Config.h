#pragma once
#include <string>

// Paramètres de simulation chargés depuis config.txt.
// Les valeurs par défaut sont utilisées si la clé est absente du fichier.
struct Config {
    size_t N        = 500;
    double boxSize  = 20.0;
    double dt       = 0.0005;
    size_t steps    = 10000;

    double initVelocity = 0.2;

    std::string potential  = "LJ";  // "LJ" (Lennard-Jones) ou "NONE" (gaz parfait)
    std::string integrator = "VV";  // "VV" (Velocity Verlet), "Euler", "RK4"

    // Paramètres Lennard-Jones : V(r) = 4ε[(σ/r)¹² - (σ/r)⁶]
    double epsilon = 1.0;  // profondeur du puits
    double sigma   = 1.0;  // distance à laquelle V = 0 ; r_min = 2^(1/6) σ
    double cutoff  = 2.5;  // rayon de troncature en unités absolues

    // Fonction de distribution radiale
    int    rdfBins  = 100;   // nombre de bins
    size_t rdfStart = 2000;  // step à partir duquel on commence l'échantillonnage
                             // (le système est supposé thermalisé)
};

Config loadConfig(const std::string& filename);