
#include "Term.h"

Term::Term(int c, int e)
{
    coeff = c;
    exp = e;
}
Term::Term()
{
    coeff = 0;
    exp = 0;
}

string Term::toString()
{
    string coeffStr = to_string(coeff);
    string expStr = "";
    if (exp != 0)
    {
        expStr += ("x^" + to_string(exp));
    }
    return coeffStr + expStr;
}
Term Term::operator*(Term t)
{
    return Term(coeff * t.coeff, exp + t.exp);
}
