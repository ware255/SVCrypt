#include <vector>
#include <iostream>
#include "Term.h"

using namespace std;
/*
    Class to provide polynomial functionality
    such as adding, multiplying and obtaining
    inverses of a polynomial

*/
class Polynomial
{
    private:
        //length of polynomial (Degree + 1)
        int length = 0;
        //Dynamic array of terms in polynomial
        //They will be in order and accessible by index 
        vector<Term> polynomial;
    public:
        //Constructors
        Polynomial();
        Polynomial(vector<Term> &poly);
        Polynomial(int* poly, int len);
        //Returns string for debugging
        string toString();
        vector<Term> getRawPoly();
        //Overrides addition
        Polynomial operator+(Polynomial &adding);
        //Overrides multiplication
        Polynomial operator*(Polynomial &p);
        void reduceCoeffMod(int x);
        void reduceExpMod(int x);


};