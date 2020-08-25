#include <iostream>
#include <vector>
#include <algorithm>
#include "Polynomial.h"
#include "Term.h"

Polynomial genRandPoly(int n, int numOnes, int numNegOnes);
Polynomial invertPoly(Polynomial poly, int n, int p);
int multInverseModPrime(int toInverse, int p);
int extenedEuclid(int a, int b, int *x, int *y);


using namespace std;
int main()
{
    int ex1[2] = {0,1};
    int ex2[4] = {0,0,0,0};
    Polynomial exPoly1(ex1, 2);
    Polynomial exPoly2(ex2, 4);
    Polynomial exPoly3 = exPoly1 * exPoly2;
    cout << exPoly1.toString() << endl;
    cout << exPoly2.toString() << endl;
    cout << exPoly3.toString() << endl;


    cout << multInverseModPrime(6,7) << endl;


    Polynomial rand = genRandPoly(10, 5, 2);
    cout << "Original Poly: " << rand.toString() << endl;
    cout << rand.getDegree() << endl;
    Polynomial inverted = invertPoly(rand, 11, 3);
    cout << "Inverse Poly: " << inverted.toString() << endl;

    cout << "GCD TEST" << endl;
    try 
    {
        int inverse = multInverseModPrime(3,4);
        cout << inverse << endl;

    }
    catch (const char* msg)
    {
        cerr << msg << endl;
    }
    return 0;
}

Polynomial genRandPoly(int n, int numOnes, int numNegOnes)
{
    srand(time(NULL));
    if (numOnes < 0 || numNegOnes < 0 || n < 0)
        throw "invalid paramaters";
    int r[n] = {};
    while ((numOnes != 0) || (numNegOnes != 0))
    {
        int randIndex = rand() % n;
        if (r[randIndex] == 0)
        {
            if(numOnes > 0)
            {
                r[randIndex] = 1;
                numOnes--;
            }
            else if (numNegOnes > 0)
            {   
                r[randIndex] = -1;
                numNegOnes--;
            }
        }
    }
    return Polynomial(r, n);
}
int multInverseModPrime(int toInverse, int p)
{
    int x, y;
    int gcd = extenedEuclid(toInverse, p, &x, &y);
    if (abs(gcd) != 1)
    {
        cout << "GCD = " << gcd << endl;
        throw "Inverse not computable";
    }
    else
    {
        //p added to force positive result
        int res = (x%p + p) % p;
        return res;
    }
    
}

// ax + by = gcd(a, b)
int extenedEuclid(int a, int b, int *x, int *y)
{
    if (a == 0)
    {
        *x = 0;
        *y = 1;
        return b;
    }
    int x1, y1;
    int gcd = extenedEuclid(b%a, a, &x1, &y1);

    //updates x and y passed
    *x = y1 - (b/a) * x1; 
    *y = x1;
    //returns gcd
    return gcd;
}
/*
    Inverts polynomial in the ring (Z/pZ)[X]/(X^N - 1)
    @input poly - polynomial to invert
    @input n    - exponent of X^N - 1 i.e. X^N = 1 therefore X^(N+1) = X^N * X = 1 * X = X
    @input p    - prime integer such that Z modulo pZ is a group
    @output     - inverse of poly
*/
Polynomial invertPoly(Polynomial poly, int n, int p)
{
    //return value
    int inversePolyArr[n] = {};
    Polynomial inversePoly(inversePolyArr, n);

    //setup
    int k = 0;
    int tempArrB[1] = {1};
    int tempArrC[1] = {0};
    int x[2] = {0,1};
    Polynomial b(tempArrB, 1);
    Polynomial c(tempArrC, 1);
    Polynomial xPoly(x, 2);
    Polynomial f = poly;

    //creates polynomial X^N - 1
    int maxPolyArr[n+1] = {};
    maxPolyArr[n] = 1;
    maxPolyArr[0] = -1;
    Polynomial g(maxPolyArr, n+1);
    int count = 10;
    cout << "Start: " << f.toString() << endl;
    cout << "X:" << g.toString() << endl;
    while(true)
    {
        while (f.getCoeff(0) == 0)
        {
            int fDegree = f.getDegree();
            cout << f.toString() << endl;
            for (int i = 1; i <= fDegree ; i++)
            {
                f.setCoeff(f.getCoeff(i), i-1); // f(x) => f(x)/x
            }
            f.setCoeff(0, fDegree);
            cout << f.toString() << endl;
            c = c * xPoly;
            k += 1;
        }
        if (f.getDegree() == 0)
        {
            break;
        }
        if (f.getDegree() < g.getDegree())
        {
            Polynomial temp = f;
            f = g;
            g = temp;
            temp = b;
            b = c;
            c = temp;

        }
        int u = f.getCoeff(0) * multInverseModPrime(g.getCoeff(0), p) % p;
        Polynomial tempG = g * u;
        Polynomial tempC = c * u;
        f = f - tempG;
        b = b - tempC;
        f.reduceCoeffMod(p);
        b.reduceCoeffMod(p);
        count--;
    }
    int j = 0;
    k %= n;
    int f_0Inverse = multInverseModPrime(f.getCoeff(0), p);
    for (int i = n-1; i >= 0; i--)
    {
        b.setCoeff(b.getCoeff(i) * f_0Inverse, i);
        j = i - k;
        if (j < 0)
        {
            j += n;
        }
        inversePoly.setCoeff(b.getCoeff(i),j);
    }

    cout << f.toString() << endl;
    return inversePoly;
}
