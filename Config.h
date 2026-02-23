#pragma once
#include <string>

struct Config {
    size_t N = 500;
    double boxSize = 20.0;
    double dt = 0.0005;
    size_t steps = 10000;

    double initVelocity = 0.2;

    // Lennard-Jones
    double epsilon = 2.0;
    double sigma = 1.0;
    double cutoff = 2.5;

    // RDF
    int rdfBins = 100;
    size_t rdfStart = 2000;
};

// charge config.txt
Config loadConfig(const std::string& filename);