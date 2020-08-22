
#include "Polynomial.h"
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

static bool compareTerm(vector<int> first, vector<int> second)
{
    int firstExponent = first[1];
    int secondExponent = second[2];
    return (firstExponent > secondExponent);
}
//Default constructor 
Polynomial::Polynomial()
{
    length = 0;
    polynomial = vector<Term>{Term()};
}
Polynomial::Polynomial(vector<Term> &poly)
{
    //sort(poly.begin(), poly.end(), compareTerm);
    polynomial = poly;
    Polynomial::length = poly.size();
}
//Constructor takes coefficient array ie {1,2,3} and returns a polynomial e.g. 1 + 2x + 3x^2
Polynomial::Polynomial(int* poly, int len)
{
    vector<Term> v;
    for (int i = 0; i < len; i++)
    {
        v.push_back(Term(poly[i], i));
    }
    polynomial = v;
    length = len;
}
//Returns string for debugging
string Polynomial::toString()
{
    string output ="";
    for(Term term : polynomial)
    {      
        output += (term.toString() + " + ");
    }
    
    //If polynomial is empty then show as 0
    if (output.compare("") == 0)
        output += "0  ";
    //Quick fix to remove trailing + 
    output.resize(output.length() - 2);
    return output;
}
//Returns raw poly representation
vector<Term> Polynomial::getRawPoly()
{
    return polynomial;
}

//Overloads addition
Polynomial Polynomial::operator+ (Polynomial &adding)
{
    vector<Term> polynomial1 = getRawPoly();
    vector<Term> polynomial2 = adding.getRawPoly();
    
    int degree1 = length;
    int degree2 = adding.length;
    int maxDegree = max(degree1,degree2);

    int ret[maxDegree] = {};
    for (int i = 0; i < maxDegree; i++)
    {
        int total = 0;
        if(i < degree1)
        {
            total += polynomial1[i].coeff;
        }
        if(i < degree2)
        {
            total += polynomial2[i].coeff;
        }
        ret[i] = total;
    }
    return Polynomial(ret, maxDegree);
}
//Overloads multiplication
Polynomial Polynomial::operator* (Polynomial &p)
{
    cout << "Hello" << endl;
    vector<Term> poly1 = getRawPoly();
    vector<Term> poly2 = p.getRawPoly();
    

    int maxLength = (length-1) + (p.length-1) + 1;
    int ret[maxLength] = {};
    for (int i = 0; i < length; i++)
    {
        for(int j = 0; j < p.length; j++)
        {
            int term = poly1[i].coeff * poly2[j].coeff;
            ret[i + j] += term;
        }
    }
    for (int i : ret)
    {
        cout << i << endl;
    }
    return Polynomial(ret, maxLength);
}
//Reduces all coefficients mod x ie 2 + 4x + 5x^2 mod 2 3 = 2 + x + 2x^2
void Polynomial::reduceCoeffMod(int x)
{
    for(Term &term : polynomial)
    {
        term.coeff %= x;
    }
}
//reduces all exponents mod x ie 2^x3 + 5x^5 mod 2 = 2x + 3x = 5x
void Polynomial::reduceExpMod(int x)
{
    for(Term &term : polynomial)
    {
        term.exp %= x;
    }
}
