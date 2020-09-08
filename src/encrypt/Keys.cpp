#include "Keys.h"
#include <math.h>
static Polynomial genRandPoly(int n, int numOnes, int numNegOnes);
static Polynomial invertPoly(Polynomial poly, int n, int p);
static Polynomial invertPolyMod3(Polynomial poly, int n);
static Polynomial invertPolyMod2(Polynomial poly, int n);
static Polynomial invertPolyModPrimePower(Polynomial poly, Polynomial inverseModp, int n, int p, int r);
static int multInverseModPrime(int toInverse, int p);
static int extenedEuclid(int a, int b, int *x, int *y);

Keys::Keys(Parameters params)
{
    srand(time(NULL));
    // int rand1 = rand() % params.getN()/4;
    // int rand2 = rand() % params.getN()/4;
    // int rand3 = rand() % params.getN()/4;
    // int rand4 = rand() % params.getN()/4;
    Polynomial f = genRandPoly(params.getN(), 2, 3);
    Polynomial g = genRandPoly(params.getN(), 4, 2);
    Polynomial invertedMod2 = invertPolyMod2(f, params.getN());
    Polynomial invertedMod3 = invertPolyMod3(f, params.getN());
    Polynomial invertedMod32 = invertPolyModPrimePower(f, invertedMod2, params.getN(), params.getX(), params.getR());
    int arr3[1] = {3};
    Polynomial p3(arr3,1);
    Polynomial h = ((p3*invertedMod32)*g).reduceExpMod(params.getN());
    h.reduceCoeffMod(params.getQ());

    publicKey = h;
    privateKeyF = f;
    privateKeyG = g;
    privateKeyFp = invertedMod3;
    privateKeyFq = invertedMod32;
    
}

Polynomial Keys::getPublicKey()
{
    return publicKey;
}
Polynomial Keys::getPrivateKeyF()
{
    return privateKeyF;
}
Polynomial Keys::getPrivateKeyG()
{
    return privateKeyG;
}
Polynomial Keys::getPrivateKeyFp()
{
    return privateKeyFp;
}
Polynomial Keys::getPrivateKeyFq()
{
    return privateKeyFq;
}

string Keys::toString()
{
    string out = "";
    out += ("H(X): " + publicKey.toString() + '\n');
    out += ("F(X): " + privateKeyF.toString() + '\n'); 
    out += ("G(X): " + privateKeyG.toString() + '\n'); 
    out += ("Fp(X): " + privateKeyFp.toString() + '\n'); 
    out += ("Fq(X): " + privateKeyFq.toString() + '\n'); 
    
    return out;
}


static Polynomial genRandPoly(int n, int numOnes, int numNegOnes)
{
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

static int multInverseModPrime(int toInverse, int p)
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
static int extenedEuclid(int a, int b, int *x, int *y)
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
static Polynomial invertPoly(Polynomial poly, int n, int p)
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
        cout << f.getDegree() << endl;
        if (f.getDegree() == 0)
        {
            break;
        }
        if (f.getDegree() < g.getDegree())
        {
            cout << "degree <" << endl;
            Polynomial temp = f;
            f = g;
            g = temp;
            temp = b;
            b = c;
            c = temp;

        }
        int u = ((f.getCoeff(0) * multInverseModPrime(g.getCoeff(0), p) % p) + p) % p;
        cout << "u:" << u << endl;
        Polynomial tempG = g * u;
        Polynomial tempC = c * u;
        cout << f.toString() << endl;
        cout << g.toString() << endl;
        f = f - tempG;
        b = b - tempC;
        f.reduceCoeffMod(p);
        g.reduceCoeffMod(p);
        b.reduceCoeffMod(p);
        c.reduceCoeffMod(p);
        count--;
    }
    int f_0Inverse = multInverseModPrime(f.getCoeff(0), p);
    b = b * f_0Inverse;
    b.reduceCoeffMod(p);

    // B(X) => X^(n-k) B(X) (mod X^N - 1)
    int XArr[n - k + 1] = {};
    XArr[n-k] = 1;
    Polynomial XPoly(XArr, n-k+1);
    b = b * XPoly;
    inversePoly = b;
    cout << f.toString() << endl;
    return inversePoly;
}
/*
    Inverts polynomial in the ring (Z/3Z)[X]/(X^N - 1)
    @input poly - polynomial to invert
    @input n    - exponent of X^N - 1 i.e. X^N = 1 therefore X^(N+1) = X^N * X = 1 * X = X
    @output     - inverse of poly
*/
static Polynomial invertPolyMod3(Polynomial poly, int n)
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
    while(true)
    {   
        while (f.getCoeff(0) == 0)
        {
            // f(x) => f(x)/x
            int fDegree = f.getDegree();
            for (int i = 1; i <= fDegree ; i++)
            {
                f.setCoeff(f.getCoeff(i), i-1); 
            }
            f.setCoeff(0, fDegree);
            // c(x) => x * c(x)
            c = c * xPoly;
            // k => k + 1
            k += 1;
        }
        if (f.getDegree() == 0 && f.getCoeff(0) != 0)
        {
            break;
        }
        //Swaps polynomials f with g / b with c
        if (f.getDegree() < g.getDegree())
        {
            Polynomial temp = f;
            f = g;
            g = temp;
            temp = b;
            b = c;
            c = temp;

        }
        if (f.getCoeff(0) == g.getCoeff(0))
        {
            f = f - g;
            b = b - c;
        }
        else
        {
            f = f + g;
            b = b + c;
        }
        //Reduce all polynomials mod 3
        f.reduceCoeffMod(3);
        g.reduceCoeffMod(3);
        b.reduceCoeffMod(3);
        c.reduceCoeffMod(3);
    }

    // B(X) => F_0 * X^(-k) B(X) (mod X^N - 1)
    int size = (((-k) % n) + n) % n;
    int XArr[size+1] = {};
    XArr[size] = 1 * f.getCoeff(0);
    Polynomial XPoly(XArr, size+1);
    b = b * XPoly;
    inversePoly = b.reduceExpMod(n);
    inversePoly.reduceCoeffMod(3);
    // Return final inverse
    return inversePoly;
}
/*
    Inverts polynomial in the ring (Z/2Z)[X]/(X^N - 1)
    @input poly - polynomial to invert
    @input n    - exponent of X^N - 1 i.e. X^N = 1 therefore X^(N+1) = X^N * X = 1 * X = X
    @output     - inverse of poly
*/
static Polynomial invertPolyMod2(Polynomial poly, int n)
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
    while(true)
    {   
        while (f.getCoeff(0) == 0)
        {
            // f(x) => f(x)/x
            int fDegree = f.getDegree();
            for (int i = 1; i <= fDegree ; i++)
            {
                f.setCoeff(f.getCoeff(i), i-1); 
            }
            f.setCoeff(0, fDegree);
            // c(x) => x * c(x)
            c = c * xPoly;
            // k => k + 1
            k += 1;
        }
        if (f.getDegree() == 0 && f.getCoeff(0) != 0)
        {
            break;
        }
        //Swaps polynomials f with g / b with c
        if (f.getDegree() < g.getDegree())
        {
            Polynomial temp = f;
            f = g;
            g = temp;
            temp = b;
            b = c;
            c = temp;

        }
        //f -> f(x) + g(x) mod 2
        //b -> b(x) + c(x) mod 2
        f = f + g;
        b = b + c;
        //Reduce all polynomials mod 2
        f.reduceCoeffMod(2);
        g.reduceCoeffMod(2);
        b.reduceCoeffMod(2);
        c.reduceCoeffMod(2);
    }

    // B(X) => F_0 * X^(-k) B(X) (mod X^N - 1)
    int size = (((-k) % n) + n) % n;
    int XArr[size+1] = {};
    XArr[size] = 1;
    Polynomial XPoly(XArr, size+1);
    b = b * XPoly;
    inversePoly = b.reduceExpMod(n);
    inversePoly.reduceCoeffMod(2);
    // Return final inverse
    return inversePoly;
}

/*
    Inverts polynomial in the ring (Z/(p^r)Z)[X]/(X^N - 1)
    @input poly         - polynomial to invert
    @input inverseModp  - inverse of poly in ring (Z/pZ)[X] (X^N -1)
    @input n    - exponent of X^N - 1 i.e. X^N = 1 therefore X^(N+1) = X^N * X = 1 * X = X
    @input p            - prime modulo of original inverse
    @input r            - power of prime
    @output             - inverse of poly mod (p^r)
*/
static Polynomial invertPolyModPrimePower(Polynomial poly, Polynomial inverseModp, int n, int p, int r)
{

    //initialise
    int q = p;
    int max = pow(p, r);
    Polynomial out = inverseModp;
    // Initialise constant f(x) = 2 outside of while loop
    int twoArr[1] = {2};
    Polynomial twoPoly = Polynomial(twoArr, 1);
    //while q < p^r
    while (q < max)
    {
        q *= 2;
        //out(x) => out(x)(2 - poly(x)out(x)) (mod q)
        Polynomial mult = poly * out;
        Polynomial twoMinusMult = twoPoly - mult;
        Polynomial final = out * twoMinusMult;
        out = final.reduceExpMod(n);
        out.reduceCoeffMod(q);
    }
    out.reduceCoeffMod(max);
    return out;
}
