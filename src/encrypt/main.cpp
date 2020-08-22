#include <iostream>
#include <vector>
#include "Polynomial.h"
#include "Term.h"

Polynomial genRandPoly(int n, int numOnes, int numNegOnes);
using namespace std;
int main()
{
    cout << "Hello World" << endl;
    int a_poly[5] = {1,3,5,0,2};
    int b_poly[2] = {0,1};
    Polynomial poly = Polynomial(a_poly,5);
    Polynomial poly2 = Polynomial(b_poly,2);

    cout << poly.toString() << endl;
    cout << poly2.toString() << endl;
    Polynomial poly3 = poly + poly2;
    cout << poly3.toString() << endl;

    Term mult = Term(1,3) * Term(2,2);
    cout << mult.toString() << endl;
    Polynomial poly4 = poly * poly2;
    cout << poly4.toString() << endl;
    poly4.reduceCoeffMod(2);
    cout << poly4.toString() << endl;

    Polynomial rand = genRandPoly(10, 0, 2);
    cout << rand.toString() << endl;
    return 0;
}

Polynomial genRandPoly(int n, int numOnes, int numNegOnes)
{
    if (numOnes < 0 || numNegOnes < 0 || n < 0)
        throw "invalid paramaters";
    int r[n] = {};
    while ((numOnes != 0) || (numNegOnes != 0))
    {
        cout << "hello" << endl;
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

Polynomial inversePoly()
{

}
