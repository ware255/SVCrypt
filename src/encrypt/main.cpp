#include <iostream>
#include <vector>
#include <algorithm>
#include "Polynomial.h"
#include "Parameters.h"
#include <math.h>
#include <sstream>
#include <bitset>
#include "Keys.h"

Polynomial genRandPoly(int n, int numOnes, int numNegOnes);
Polynomial invertPoly(Polynomial poly, int n, int p);
Polynomial invertPolyMod3(Polynomial poly, int n);
Polynomial invertPolyMod2(Polynomial poly, int n);
Polynomial invertPolyModPrimePower(Polynomial poly, Polynomial inverseModp, int n, int p, int r);
int multInverseModPrime(int toInverse, int p);
int extenedEuclid(int a, int b, int *x, int *y);
Polynomial encryptPoly(Polynomial toEncrypt, Polynomial publicKey);
Polynomial decryptPoly(Polynomial toDecrypt, Polynomial privateKeyF, Polynomial privateKeyFp);
vector<Polynomial> encryptString(string toEncrypt, Polynomial publicKey);
string decryptString(vector<Polynomial> toDecrypt, Polynomial privateKeyF, Polynomial privateKeyFp);
vector<Polynomial> stringToBinPolys(string in, int n);
string binPolysToString(vector<Polynomial> in, int n);
string polysToBinaryString(vector<Polynomial> in, int n);

using namespace std;
int main()
{
    Parameters params(3,2,5,32,11);
    Keys keys(params);
    // int fArr[11] = {-1, 1 ,1, 0, -1, 0, 1, 0, 0, 1 , -1};
    // int gArr[11] = {-1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1};
    // Polynomial f(fArr, params.getN());
    // Polynomial g(gArr, params.getN());
    // cout << "F(X): " << f.toString() << endl;
    // cout << "G(X): " << g.toString() << endl;
    // Polynomial invertedMod2 = invertPolyMod2(f, params.getN());
    // Polynomial invertedMod3 = invertPolyMod3(f, params.getN());
    // Polynomial invertedMod32 = invertPolyModPrimePower(f, invertedMod2, params.getN(), params.getX(), params.getR());
    // cout << "FP(X): " << invertedMod3.toString() << endl;
    // cout << "FQ(X): " << invertedMod32.toString() << endl;
    // int arr3[1] = {3};
    // Polynomial p3(arr3,1);
    // Polynomial publicKey = ((p3*invertedMod32)*g).reduceExpMod(params.getN());
    // publicKey.reduceCoeffMod(params.getQ());
    // cout << "H(X): " << publicKey.toString() << endl;

    int mArr[] = {-1,0,0,1,-1,0,0,0,-1,1,1};
    Polynomial message(mArr, 11);
    Polynomial encrypted = encryptPoly(message, keys.getPublicKey());
    Polynomial decrypted = decryptPoly(encrypted, keys.getPrivateKeyF(), keys.getPrivateKeyFp());

    cout << "STRING" << endl;
    string in = "Hello world, how are you?";
    vector<Polynomial> enc = encryptString(in, keys.getPublicKey());
    cout << "Encrypted" << polysToBinaryString(enc, 11) << endl;
    string dec = decryptString(enc, keys.getPrivateKeyF(), keys.getPrivateKeyFp());
    cout << dec << endl;
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
Polynomial invertPolyMod3(Polynomial poly, int n)
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
Polynomial invertPolyMod2(Polynomial poly, int n)
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
Polynomial invertPolyModPrimePower(Polynomial poly, Polynomial inverseModp, int n, int p, int r)
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

/* 
    Function to encrypt a given polynomial with a recipients public key

    Encrypted = r * publicKey + toEncypt (mod q) where r is a poly with small coeffs

    @input toEncrypt - Polynomial to encrypt
    @input publicKey - Public key of recipient for NTRU encryption
    @output Polynomial encrypted with public key


*/
Polynomial encryptPoly(Polynomial toEncrypt, Polynomial publicKey)
{
    int rArr[] = {-1,0,1,1,1,-1,0,-1};
    Polynomial r(rArr,8);
    Polynomial rPubKey = (r * publicKey).reduceExpMod(11);
    Polynomial encrypted = rPubKey + toEncrypt;
    encrypted.reduceCoeffMod(32);
    return encrypted;
}

/* 
    Function to decrypt a given encrypted polynomial

    Procedure:
        a = privateKeyF * toDecrypt (mod q)
        b = a (mod p) = privateKeyF * message (modP)
        c = b * privateKeyFq = (privateKeyF * privateKeyFq) * message = message (mod p)

    @input toDecrypt - Polynomial to decrypt, encrypted with users public key
    @input privateKeyF - Private Key F generated by user
    @input privateKeyFp - Inverse of private key F (mod p)
    @output Message polynomial


*/
Polynomial decryptPoly(Polynomial toDecrypt, Polynomial privateKeyF, Polynomial privateKeyFp)
{
    Polynomial a = (privateKeyF * toDecrypt).reduceExpMod(11);
    a.reduceCoeffMidMod(32);
    a.reduceCoeffMidMod(3);
    Polynomial c = (a * privateKeyFp).reduceExpMod(11);
    c.reduceCoeffMidMod(3);
    return c;
}

vector<Polynomial> stringToBinPolys(string in, int n)
{
    vector<Polynomial> polyVec;
    string binary = "";
    for (int i = 0; i < in.length(); i++)
    {
        binary += bitset<8>(in[i]).to_string();
    }
    int numBits = in.length() * 8;
    int numPolys = ceil((float)numBits / n);
    for (int i = 0; i < numPolys; i++)
    {
        int polyArr[n] = {};
        for (int j = 0; j < n; j++)
        {
            int index = i*n + j;
            if (index < numBits)
            {
                polyArr[j] = binary[i*n + j] - 48;
            }
            else
            {
                polyArr[j] = 0;
            }
        }
        Polynomial poly(polyArr, n);
        polyVec.push_back(poly);

    }
    return polyVec;
}

string binPolysToString(vector<Polynomial> in, int n)

{
    string bitStr = "";
    string out = "";
    for (Polynomial poly : in)
    {
        int length = poly.getLength();
        for (int i = 0; i < n; i++)
        {
            if (i < length)
            {
                bitStr += (poly.getCoeff(i) + 48); 
            } 
            else
            {
                bitStr += '0';
            }
                   
        }
    }
    stringstream stream(bitStr);
    while(stream.good())
    {
        bitset<8> bits;
        stream >> bits;
        char c = char(bits.to_ulong());
        out += c;
    }
    return out;
}

string polysToBinaryString(vector<Polynomial> in, int n)
{
    string bitStr = "";
    for (Polynomial poly : in)
    {
        int length = poly.getLength();
        for (int i = 0; i < n; i++)
        {
            if (i < length)
            {
                bitset<8>bit(poly.getCoeff(i));
                bitStr += bit.to_string();
            } 
            else
            {
                bitStr += '0';
            }
                   
        }
    }
    return bitStr;
}


vector<Polynomial> encryptString(string toEncrypt, Polynomial publicKey)
{
    vector<Polynomial> plainPolys = stringToBinPolys(toEncrypt, 11);
    vector<Polynomial> encryptedPolys;
    for (Polynomial p : plainPolys)
    {
        Polynomial enc = encryptPoly(p, publicKey);
        encryptedPolys.push_back(enc);
    }
    return encryptedPolys;
}

string decryptString(vector<Polynomial> toDecrypt, Polynomial privateKeyF, Polynomial privateKeyFp)
{
    vector<Polynomial> plainPolys;
    for (Polynomial p : toDecrypt)
    {
        Polynomial dec = decryptPoly(p, privateKeyF, privateKeyFp);
        plainPolys.push_back(dec);
    }
    string decrypted = binPolysToString(plainPolys, 11);
    return decrypted;    
}