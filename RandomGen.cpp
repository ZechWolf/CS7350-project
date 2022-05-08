#include "RandomGen.h"

double RandomGen::randRealUniform()
{
    return uniformReal(generator);
}

int RandomGen::randSkewed()
{
    return 0;
}

int RandomGen::randUniform()
{
    return uniform(generator);
}

int RandomGen::randNormal()
{
    //Uses Box-Mueller transformation to generate normal distribution
    //Box-Mueller generates two values, one is stored for later use and one is returned
    //Inspired by code from here: https://statisticsblog412536603.wordpress.com/2019/10/31/do-research-about-box-muller-transform/

    int result;
    if (z1_stored == -1)
    {
        double u1 = randRealUniform();
        double u2 = randRealUniform();
        int z0 = sqrt(-2 * log(u1)) * cos(2*M_PI*u2);
        int z1 = sqrt(-2 * log(u1)) * sin(2*M_PI*u2);
        z1_stored = z1;
        result = z0;
    }
    else
    {
        result = z1_stored;
        z1_stored = -1;
    }

    return result * (max / 3) + (max / 2); //normal with mean of max/2 and sigma max/5

}