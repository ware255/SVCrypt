#pragma once

#include <iostream>

using namespace std;
class Term
{   
    public:
        int coeff;
        int exp;
        Term(int c, int e);
        Term();
        string toString();
        Term operator*(Term t);
};