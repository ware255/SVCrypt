#include <vector>
#include <iostream>

using namespace std;
class Polynomial
{
    private:
        int length = 0;
        vector<vector<int>> polynomial;
    public:
        Polynomial();
        Polynomial(vector<vector<int>> &poly);
        string toString();
        int degree();


};