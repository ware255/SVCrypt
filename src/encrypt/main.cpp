#include <iostream>
#include <vector>
#include "Polynomial.h"
#include "Term.h"


using namespace std;
int main()
{
    cout << "Hello World" << endl;
    vector<Term> vPoly;
    vPoly.push_back(Term(2,3));
    vPoly.push_back(Term(1,5));
    vPoly.push_back(Term(4,6));
    vPoly.push_back(Term(3,0));
    Polynomial poly = Polynomial(vPoly);
    Polynomial poly2 = Polynomial(vPoly);

    cout << poly.toString() << endl;
    cout << poly2.toString() << endl;
    Polynomial poly3 = poly + poly2;
    cout << poly3.toString() << endl;

    Term mult = Term(1,3) * Term(2,2);
    cout << mult.toString();

    return 0;
}
