#include "Parameters.h"

Parameters::Parameters(int inp, int inq, int inn)
{
    p = inp;
    q = inq;
    n = inn;
}
Parameters::Parameters()
{
    p = 0;
    q = 0;
    n = 0;
}
int Parameters::getP()
{
    return p;
}
int Parameters::getQ()
{
    return q;
}
int Parameters::getN()
{
    return n;
}