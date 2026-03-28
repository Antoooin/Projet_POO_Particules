#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

static std::string trim(std::string s)
{
    s.erase(0, s.find_first_not_of(" \t\r\n"));
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
    return s;
}

static std::string stripComment(std::string s)
{
    auto pos = s.find("//");
    if(pos != std::string::npos)
        s.erase(pos);
    return s;
}

Config loadConfig(const std::string& filename)
{
    Config cfg;
    std::ifstream file(filename);

    if(!file) {
        std::cerr << "Warning: config file not found, using defaults\n";
        return cfg;
    }

    std::string line;
    while(std::getline(file, line))
    {
        line = trim(stripComment(line));

        if(line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        std::string key, value;

        if(std::getline(iss, key, '=') &&
           std::getline(iss, value))
        {
            key   = trim(key);
            value = trim(value);

            std::stringstream val(value);

            if(key == "N") val >> cfg.N;
            else if(key == "boxSize") val >> cfg.boxSize;
            else if(key == "dt") val >> cfg.dt;
            else if(key == "steps") val >> cfg.steps;
            else if(key == "initVelocity") val >> cfg.initVelocity;
            else if(key == "potential") cfg.potential = value;
            else if(key == "integrator") cfg.integrator = value;
            else if(key == "epsilon") val >> cfg.epsilon;
            else if(key == "sigma") val >> cfg.sigma;
            else if(key == "cutoff") val >> cfg.cutoff;
            else if(key == "rdfBins") val >> cfg.rdfBins;
            else if(key == "rdfStart") val >> cfg.rdfStart;
        }
    }

    return cfg;
}