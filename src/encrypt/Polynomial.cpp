
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
    polynomial = vector<int>{};
}
Polynomial::Polynomial(vector<int> &poly)
{
    polynomial = poly;
    length = poly.size();
}
//Constructor takes coefficient array ie {1,2,3} and returns a polynomial e.g. 1 + 2x + 3x^2
Polynomial::Polynomial(int* poly, int len)
{
    int degree = getDegree(poly, len);
    vector<int> v;
    for (int i = 0; i < degree + 1; i++)
    {
        v.push_back(poly[i]);
    }
    polynomial = v;
    length = degree + 1;
}

int Polynomial::getCoeff(int index)
{
    if (index < length)
    {
        return polynomial[index];
    }
    else
    {
        cout << "Index " << index << " out of bounds" << endl;
        throw "Index out of bounds";
    }
    
}

void Polynomial::setCoeff(int toSet, int index)
{
    if (index < length)
    {
        polynomial[index] = toSet;
    }
    else
    {
        cout << "Index out of bounds" << endl;
        throw "Index out of bounds";
    }
    
}
//Returns string for debugging
string Polynomial::toString()
{
    string output ="";
    for(int i = 0; i < length; i++)
    {   
        int coeff = polynomial[i];
        if (true)
        {
            if (i != 0)
            {
                output += (to_string(coeff) + "x^" + to_string(i)  + " + ");

            }
            else
            {
                output += (to_string(coeff) + " + ");
            }
        }
    }
    
    //If polynomial is empty then show as 0
    if (output.compare("") == 0)
        output += "0  ";
    //Quick fix to remove trailing + 
    output.resize(output.length() - 2);
    return output;
}
//Returns raw poly representation
vector<int> Polynomial::getRawPoly()
{
    return polynomial;
}

//Overloads addition
Polynomial Polynomial::operator+ (Polynomial &adding)
{
    vector<int> polynomial1 = getRawPoly();
    vector<int> polynomial2 = adding.getRawPoly();
    
    int degree1 = length;
    int degree2 = adding.length;
    int maxDegree = max(degree1,degree2);

    int ret[maxDegree] = {};
    for (int i = 0; i < maxDegree; i++)
    {
        int total = 0;
        if(i < degree1)
        {
            total += polynomial1[i];
        }
        if(i < degree2)
        {
            total += polynomial2[i];
        }
        ret[i] = total;
    }
    return Polynomial(ret, maxDegree);
}

Polynomial Polynomial::operator-(Polynomial &subbing)
{
     vector<int> polynomial1 = getRawPoly();
    vector<int> polynomial2 = subbing.getRawPoly();
    
    int degree1 = length;
    int degree2 = subbing.length;
    int maxDegree = max(degree1,degree2);

    int ret[maxDegree] = {};
    for (int i = 0; i < maxDegree; i++)
    {
        int total = 0;
        if(i < degree1)
        {
            total += polynomial1[i];
        }
        if(i < degree2)
        {
            total -= polynomial2[i];
        }
        ret[i] = total;
    }
    return Polynomial(ret, maxDegree);
}

//Overloads multiplication
Polynomial Polynomial::operator* (Polynomial &p)
{
    vector<int> poly1 = getRawPoly();
    vector<int> poly2 = p.getRawPoly();
    

    int maxLength = (length-1) + (p.length-1) + 1;
    int ret[maxLength] = {};
    for (int i = 0; i < length; i++)
    {
        for(int j = 0; j < p.length; j++)
        {
            int term = poly1[i] * poly2[j];
            ret[i + j] += term;
        }
    }

    return Polynomial(ret, maxLength);
}

Polynomial Polynomial::operator*(int &x)
{
    int ret[length] = {};
    for (int i = 0; i < length; i++)
    {
        ret[i] = (getCoeff(i) * x);
    }
    return Polynomial(ret, length);
}

//Reduces all coefficients mod x ie 2 + 4x + 5x^2 mod 3 = 2 + x + 2x^2
void Polynomial::reduceCoeffMod(int x)
{
    for(int i = 0; i < length; i++)
    {
        polynomial[i] = ((polynomial[i] % x) + x) % x;
    }
}
//Reduces all coefficients to x/2 either side of 0 ie 2 + 4x + 5x^2 mod 3 = -1 + x + -1x^2
void Polynomial::reduceCoeffMidMod(int x)
{
    for(int i = 0; i < length; i++)
    {
        int temp = polynomial[i];
        temp = ((temp % x) + x) % x;
        if (temp > x/2)
            temp = temp - x;
        polynomial[i] = temp;
    }
}

Polynomial Polynomial::reduceExpMod(int x)
{
    int newPoly[x] = {};
    for(int i = 0; i < length; i++)
    {
        newPoly[i%x] += polynomial[i];
    }
    return Polynomial(newPoly, x);
}
int Polynomial::getDegree()
{
    int degree = 0;
    for(int i = 0; i < length; i++)
    {
        if (polynomial[i] != 0)
        {
            degree = i;
        }
    }
    return degree;
}

int Polynomial::getDegree(int* arr, int length)
{
    int degree = 0;
    for(int i = 0; i < length; i++)
    {
        if (arr[i] != 0)
        {
            degree = i;
        }
    }
    return degree;
}


