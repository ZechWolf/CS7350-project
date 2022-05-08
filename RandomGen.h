#pragma once
#include <random>
#include <math.h>
//#include <math.h>

//Code inspired by: https://www.cplusplus.com/reference/random/

class RandomGen
{

private:

    int max;
    std::mt19937 generator;
    std::uniform_int_distribution<int> uniform;
    std::uniform_real_distribution<double> uniformReal;

    int z1_stored = -1;

    double randRealUniform();

public:

    RandomGen(int max): generator(time(NULL)), uniform(0, max), uniformReal(0.0, 1.0), max(max) {}

    int randUniform();
    int randSkewed();
    int randNormal();

};