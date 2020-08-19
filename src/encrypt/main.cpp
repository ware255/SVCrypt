#include <iostream>
#include <vector>
#include "Polynomial.h"


using namespace std;
int main()
{
    cout << "Hello World" << endl;
    vector<vector<int>> vPoly;
    vPoly.push_back(vector<int>{2,3});
    vPoly.push_back(vector<int>{1,5});
    Polynomial poly = Polynomial(vPoly);
    cout << poly.toString() << endl;
    return 0;
}
