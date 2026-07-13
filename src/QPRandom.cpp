#include "QPRandom.hpp"

QPRandom::QPRandom(unsigned long seed): fEngine(seed){;}

void QPRandom::SetSeed(unsigned long seed){
    fEngine.seed(seed);
}

double QPRandom::Uniform(double a, double b){
    std::uniform_real_distribution<double> dist(a, b);
    return dist(fEngine);
}

double QPRandom::Gaus(double mean, double sigma){
    std::normal_distribution<double> dist(mean, sigma);
    return dist(fEngine);
}
