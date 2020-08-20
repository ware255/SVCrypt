#include <vector>
#include <iostream>
#include "Term.h"

using namespace std;
class Polynomial
{
    private:
        int length = 0;
        vector<Term> polynomial;
    public:
        Polynomial();
        Polynomial(vector<Term> &poly);
        string toString();
        Polynomial operator+ (Polynomial &first);
        vector<Term> getRawPoly();

};