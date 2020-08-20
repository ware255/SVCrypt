
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

Polynomial::Polynomial()
{
    length = 0;
}
Polynomial::Polynomial(vector<Term> &poly)
{
    //sort(poly.begin(), poly.end(), compareTerm);
    polynomial = poly;
    Polynomial::length = poly.size();
}
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

vector<Term> Polynomial::getRawPoly()
{
    return polynomial;
}
Polynomial Polynomial::operator+ (Polynomial &adding)
{
    vector<Term> ret;
    vector<Term> polynomial1 = getRawPoly();
    vector<Term> polynomial2 = adding.getRawPoly();
    

    vector<Term>::iterator it1 = polynomial.begin();
    vector<Term>::iterator it2 = polynomial2.begin();
    while (it1 != polynomial1.end() && it2 != polynomial2.end())
    {
        Term current1 = (*it1);
        Term current2 = (*it2);
        int degree1 = current1.exp;
        int degree2 = current2.exp;
        if (degree1 > degree2)
        {
            ret.push_back(current1);
            advance(it1,1);
        }
        if (degree1 < degree2)
        {
            ret.push_back(current2);
            advance(it2,1);
        }
        if (degree1 == degree2)
        {
            int coeff = current1.coeff + current2.coeff;
            //cout << (*it1)[0] << endl;
            Term temp(coeff, degree1);
            ret.push_back(temp);
            advance(it1,1);
            advance(it2,1);
        }
    }
    return Polynomial(ret);
}

