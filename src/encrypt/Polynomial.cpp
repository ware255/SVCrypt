
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
Polynomial::Polynomial(vector<vector<int>> &poly)
{
    //sort(poly.begin(), poly.end(), compareTerm);
    polynomial = poly;
    Polynomial::length = poly.size();
}
string Polynomial::toString()
{
    string output ="";
    for(vector<int> const& term : polynomial)
    {      
        output += (to_string(term[0]) + "x^" + to_string(term[1]) + " + ");
    }
    output.resize(output.length() - 2);
    return output;
}
int Polynomial::degree()
{
    return polynomial[0][1];
}


