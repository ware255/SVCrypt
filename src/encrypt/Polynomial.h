#include <vector>
#include <iostream>
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
        vector<int> polynomial;
    public:
        //Constructors
        Polynomial();
        Polynomial(vector<int> &poly);
        Polynomial(int* poly, int len);
        //Returns string for debugging
        string toString();
        vector<int> getRawPoly();
        //Overrides addition
        Polynomial operator+(Polynomial &adding);
        //Overrides subtraction
        Polynomial operator-(Polynomial &subbing);
        //Overrides multiplication
        Polynomial operator*(Polynomial &p);
        //Overrides scalar multiplication;
        Polynomial operator*(int &x);
        void reduceCoeffMod(int x);
        int getCoeff(int index);
        void setCoeff(int toSet, int index);
        int getDegree();
        int getDegree(int* arr, int length);

};