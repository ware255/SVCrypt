#ifndef _NTRU_H_
#define _NTRU_H_

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <bitset>
#include <algorithm>

#include <cmath>

class Parameters {
    int p;
    int x;
    int r;
    int q;
    int n;
public:
    Parameters(int inp, int inx, int inr, int inq, int inn) {
    	p = inp;
        x = inx;
        r = inr;
        q = inq;
        n = inn;
    }

    Parameters() {
        p = 0;
        x = 0;
        r = 0;
        q = 0;
        n = 0;
    }

    int getP() { return p; }
    int getX() { return x; }
    int getR() { return r; }
    int getQ() { return q; }
    int getN() { return n; }
};

class Polynomial {
    //length of polynomial (Degree + 1)
    int length = 0;
    //Dynamic array of terms in polynomial
    //They will be in order and accessible by index 
    std::vector<int> polynomial;
public:
    //Constructors
    Polynomial() {
        length = 0;
        polynomial = {};
    }

    Polynomial(std::vector<int> &poly) {
        polynomial = poly;
        length = poly.size();
    }

    //Constructor takes coefficient array ie {1,2,3} and returns a polynomial e.g. 1 + 2x + 3x^2
    Polynomial(int* poly, int len) {
        int degree = getDegree(poly, len);
        std::vector<int> v;
        for (int i = 0; i < degree + 1; i++)
            v.push_back(poly[i]);
        polynomial = v;
        length = degree + 1;
    }

    //Returns string for debugging
    std::string toString() {
        std::string output ="";
        for(int i = 0; i < length; i++)
        {   
            int coeff = polynomial[i];
            if (true)
            {
                if (i != 0)
                    output += (std::to_string(coeff) + "x^" + std::to_string(i)  + " + ");
                else
                    output += (std::to_string(coeff) + " + ");
            }
        }
    
        //If polynomial is empty then show as 0
        if (output.compare("") == 0)
            output += "0  ";
        //Quick fix to remove trailing + 
        output.resize(output.length() - 2);
        return output;
    }

    std::vector<int> getRawPoly() { return polynomial; }

    //Overrides addition
    Polynomial operator+(Polynomial &adding) {
        std::vector<int> polynomial1 = getRawPoly();
        std::vector<int> polynomial2 = adding.getRawPoly();
        
        int degree1 = length;
        int degree2 = adding.length;
        int maxDegree = std::max(degree1,degree2);

        int ret[maxDegree] = {};
        for (int i = 0; i < maxDegree; i++)
        {
            int total = 0;
            if(i < degree1)
                total += polynomial1[i];
            if(i < degree2)
                total += polynomial2[i];
            ret[i] = total;
        }
        return Polynomial(ret, maxDegree);
    }

    //Overrides subtraction
    Polynomial operator-(Polynomial &subbing) {
        std::vector<int> polynomial1 = getRawPoly();
        std::vector<int> polynomial2 = subbing.getRawPoly();
        
        int degree1 = length;
        int degree2 = subbing.length;
        int maxDegree = std::max(degree1,degree2);

        int ret[maxDegree] = {};
        for (int i = 0; i < maxDegree; i++)
        {
            int total = 0;
            if(i < degree1)
                total += polynomial1[i];
            if(i < degree2)
                total -= polynomial2[i];
            ret[i] = total;
        }
        return Polynomial(ret, maxDegree);
    }

    //Overrides multiplication
    Polynomial operator*(Polynomial &p) {
        std::vector<int> poly1 = getRawPoly();
        std::vector<int> poly2 = p.getRawPoly();

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

    //Overrides scalar multiplication;
    Polynomial operator*(const int x) {
        int ret[length] = {};
        for (int i = 0; i < length; i++)
            ret[i] = (getCoeff(i) * x);
        return Polynomial(ret, length);
    }

    //Reduces all coefficients mod x ie 2 + 4x + 5x^2 mod 3 = 2 + x + 2x^2
    void reduceCoeffMod(int x) {
        for(int i = 0; i < length; i++)
            polynomial[i] = ((polynomial[i] % x) + x) % x;
    }

    //Reduces all coefficients to x/2 either side of 0 ie 2 + 4x + 5x^2 mod 3 = -1 + x + -1x^2
    void reduceCoeffMidMod(int x) {
        for(int i = 0; i < length; i++)
        {
            int temp = polynomial[i];
            temp = ((temp % x) + x) % x;
            if (temp > x/2)
                temp = temp - x;
            polynomial[i] = temp;
        }
    }

    Polynomial reduceExpMod(int x) {
        int newPoly[x] = {};
        for(int i = 0; i < length; i++)
            newPoly[i % x] += polynomial[i];
        return Polynomial(newPoly, x);
    }

    int getCoeff(int index) {
        if (index < length)
            return polynomial[index];
        else
        {
#ifdef DEBUG
            std::cout << "Index " << index << " out of bounds" << std::endl;
#endif
            throw "Index out of bounds";
        }
    }

    void setCoeff(int toSet, int index) {
        if (index < length)
            polynomial[index] = toSet;
        else
        {
#ifdef DEBUG
            std::cout << "Index out of bounds" << std::endl;
#endif
            throw "Index out of bounds";
        }
    }

    int getDegree() {
        int degree = 0;
        for (int i = 0; i < length; i++)
            if (polynomial[i] != 0)
                degree = i;
        return degree;
    }

    int getDegree(int* arr, int length) {
        int degree = 0;
        for (int i = 0; i < length; i++)
            if (arr[i] != 0)
                degree = i;
        return degree;
    }

    int getLength() { return length; }
};

namespace Math {
    static Polynomial genRandPoly(int n, int numOnes, int numNegOnes) {
        if (numOnes < 0 || numNegOnes < 0 || n < 0)
            throw "invalid paramaters";
        std::random_device rd;
        std::mt19937 mt(rd());
        //std::uniform_int_distribution<int> dist(8192, 65535);
        int r[n] = {};
        while ((numOnes != 0) || (numNegOnes != 0))
        {
            int randIndex = mt() % n;
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
        @input p    - mod
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
        while(true)
        {   
            while (f.getCoeff(0) == 0)
            {
                // f(x) => f(x)/x
                int fDegree = f.getDegree();
                for (int i = 1; i <= fDegree ; i++)
                    f.setCoeff(f.getCoeff(i), i-1);
                f.setCoeff(0, fDegree);
                // c(x) => x * c(x)
                c = c * xPoly;
                // k => k + 1
                k += 1;
            }
            if (f.getDegree() == 0 && f.getCoeff(0) != 0)
                break;
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
            f.reduceCoeffMod(p);
            g.reduceCoeffMod(p);
            b.reduceCoeffMod(p);
            c.reduceCoeffMod(p);
        }

        // B(X) => F_0 * X^(-k) B(X) (mod X^N - 1)
        int size = (((-k) % n) + n) % n;
        int XArr[size+1] = {};
        XArr[size] = 1 * f.getCoeff(0);
        Polynomial XPoly(XArr, size+1);
        b = b * XPoly;
        inversePoly = b.reduceExpMod(n);
        inversePoly.reduceCoeffMod(p);
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
        int max = std::pow(p, r);
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

    //Convers a string to a vector of degree N polys with coeffs in {0,1}
    std::vector<Polynomial> stringToBinPolys(std::string in, int n)
    {
        std::vector<Polynomial> polyVec;
        std::string binary = "";
        for (size_t i = 0; i < in.length(); i++)
            binary += std::bitset<8>(in[i]).to_string();
        int numBits = in.length() * 8;
        int numPolys = ceil((float)numBits / n);
        for (int i = 0; i < numPolys; i++)
        {
            int polyArr[n] = {};
            for (int j = 0; j < n; j++)
            {
                int index = i*n + j;
                if (index < numBits)
                    polyArr[j] = binary[i*n + j] - 48;
                else
                    polyArr[j] = 0;
            }
            Polynomial poly(polyArr, n);
            polyVec.push_back(poly);
        }
        return polyVec;
    }

    //Convers a vector of degree N polys with coeffs in {0,1} to a string
    std::string binPolysToString(std::vector<Polynomial> in, int n)
    {
        std::string bitStr = "";
        std::string out = "";
        for (Polynomial poly : in)
        {
            int length = poly.getLength();
            for (int i = 0; i < n; i++)
            {
                if (i < length)
                    bitStr += (poly.getCoeff(i) + 48); 
                else
                    bitStr += '0';     
            }
        }
        std::stringstream stream(bitStr);
        while(stream.good())
        {
            std::bitset<8> bits;
            stream >> bits;
            char c = char(bits.to_ulong());
            out += c;
        }
        return out;
    }

    //Convers a vector of degree N polys to a binary string
    std::string polysToBinaryString(std::vector<Polynomial> in, int n)
    {
        std::string bitStr = "";
        for (Polynomial poly : in)
        {
            int length = poly.getLength();
            for (int i = 0; i < n; i++)
            {
                if (i < length)
                {
                    std::bitset<8>bit(poly.getCoeff(i));
                    bitStr += bit.to_string();
                }
                else
                    bitStr += '0';
            }
        }
        return bitStr;
    }

}; // Math namespace

class Keys {
    Polynomial publicKey;
    Polynomial privateKeyF;
    Polynomial privateKeyG;
    Polynomial privateKeyFp;
    Polynomial privateKeyFq;
public:
    Keys(Parameters params) {
        Polynomial f = Math::genRandPoly(params.getN(), 2, 3);
        Polynomial g = Math::genRandPoly(params.getN(), 4, 2);
        Polynomial invertedMod2 = Math::invertPoly(f, params.getN(), 2);
        Polynomial invertedMod3 = Math::invertPoly(f, params.getN(), 3);
        Polynomial invertedMod32 = Math::invertPolyModPrimePower(f, invertedMod2, params.getN(), params.getX(), params.getR());
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

    Polynomial getPublicKey() { return publicKey; }
    Polynomial getPrivateKeyF() { return privateKeyF; }
    Polynomial getPrivateKeyG() { return privateKeyG; }
    Polynomial getPrivateKeyFp() { return privateKeyFp; }
    Polynomial getPrivateKeyFq() { return privateKeyFq; }

    std::string toString() {
        std::string out = "";
        out += ("H(X): " + publicKey.toString() + '\n');
        out += ("F(X): " + privateKeyF.toString() + '\n');
        out += ("G(X): " + privateKeyG.toString() + '\n');
        out += ("Fp(X): " + privateKeyFp.toString() + '\n');
        out += ("Fq(X): " + privateKeyFq.toString() + '\n');
        return out;
    }
};

namespace NTRU {
    /* 
        Function to encrypt a given polynomial with a recipients public key

        Encrypted = r * publicKey + toEncypt (mod q) where r is a poly with small coeffs

        @input toEncrypt - Polynomial to encrypt
        @input publicKey - Public key of recipient for NTRU encryption
        @output Polynomial encrypted with public key
    */
    Polynomial encryptPoly(Polynomial toEncrypt, Polynomial publicKey, Parameters p)
    {
        int rArr[] = {-1,0,1,1,1,-1,0,-1};
        Polynomial r(rArr,8);
        Polynomial rPubKey = (r * publicKey).reduceExpMod(p.getN());
        Polynomial encrypted = rPubKey + toEncrypt;
        encrypted.reduceCoeffMod(p.getQ());
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
    Polynomial decryptPoly(Polynomial toDecrypt, Polynomial privateKeyF, Polynomial privateKeyFp, Parameters p)
    {
        Polynomial a = (privateKeyF * toDecrypt).reduceExpMod(p.getN());
        a.reduceCoeffMidMod(p.getQ());
        a.reduceCoeffMidMod(p.getP());
        Polynomial c = (a * privateKeyFp).reduceExpMod(p.getN());
        c.reduceCoeffMidMod(p.getP());
        return c;
    }

    //Converts a string into multiple polynomials of degree N, then encrypts with public key
    std::vector<Polynomial> encryptString(std::string toEncrypt, Polynomial publicKey, Parameters param)
    {
        std::vector<Polynomial> plainPolys = Math::stringToBinPolys(toEncrypt, param.getN());
        std::vector<Polynomial> encryptedPolys;
        for (Polynomial p : plainPolys)
        {
            Polynomial enc = encryptPoly(p, publicKey, param);
            encryptedPolys.push_back(enc);
        }
        return encryptedPolys;
    }

    //Decrypts multiple polynomials of fegree N with private key and returns a string
    std::string decryptString(std::vector<Polynomial> toDecrypt, Polynomial privateKeyF, Polynomial privateKeyFp, Parameters param)
    {
        std::vector<Polynomial> plainPolys;
        for (Polynomial p : toDecrypt)
        {
            Polynomial dec = decryptPoly(p, privateKeyF, privateKeyFp, param);
            plainPolys.push_back(dec);
        }
        return Math::binPolysToString(plainPolys, param.getN());
    }

}; // NTRU namespace

#endif
