#include "Parameters.h"

Parameters::Parameters(int inp, int inx, int inr, int inq, int inn)
{
    p = inp;
    x = inx;
    r = inr;
    q = inq;
    n = inn;
}
Parameters::Parameters()
{
    p = 0;
    x = 0;
    r = 0;
    q = 0;
    n = 0;
}
int Parameters::getP()
{
    return p;
}
int Parameters::getX()
{
    return x;
}
int Parameters::getR()
{
    return r;
}
int Parameters::getQ()
{
    return q;
}
int Parameters::getN()
{
    return n;
}