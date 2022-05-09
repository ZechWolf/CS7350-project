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

int RandomGen::randNormalInRange()
{
    //Uses Box-Mueller transform to generate normal distribution
    //Box-Mueller generates two values, one is stored for later use and one is returned immediately
    //Inspired by code from here: https://statisticsblog412536603.wordpress.com/2019/10/31/do-research-about-box-muller-transform/

    //This output could be outside the specifed range, so randNormal() resamples until it gets a suitable value

    double result = -1.0;
    if (z1_stored == -1)
    {
        double u1 = randRealUniform();
        double u2 = randRealUniform();
        double z0 = sqrt(-2 * log(u1)) * cos(2*M_PI*u2);
        double z1 = sqrt(-2 * log(u1)) * sin(2*M_PI*u2);
        z1_stored = z1;
        if (z0 >= 0 && z0 <= max) //make sure its in the correct range
            result = z0;
    }
    else
    {
        result = z1_stored;
        z1_stored = -1;
    }

    return result * (max / 3) + (max / 2); //normal with mean of max/2 and sigma max/3
}

int RandomGen::randNormal()
{
    int result = -1;
    while (result < 0 || result > max)
        result = randNormalInRange();

    return result;

}