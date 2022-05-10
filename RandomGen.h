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

    double z1_stored = -1;
    double* ranges;

    double randRealUniform();
    int randNormalInRange();

public:

    RandomGen(int max): generator(time(NULL)), uniform(0, max), uniformReal(0.0, 1.0), max(max)
    {
        //Create ranges array for probability ranges of skewed distribution
        ranges = new double[max + 1];
        ranges[0] = 0;
        ranges[max + 1] = 1;

        double s = (max*(max + 1)) / 2;
        for (int i = 1; i < max + 1; i++)
        {
            ranges[i] = ranges[i-1] + ((max - i + 1) / s);
            printf("%f, ", ranges[i]);
        }
    }
    ~RandomGen();

    int randUniform();
    int randSkewed();
    int randNormal();

};