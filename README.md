# SVCrypt
C++ implementation of NTRU Cryptosystem\
https://en.wikipedia.org/wiki/NTRUEncrypt

**WARNING: Do not use this to encrypt anything**\
It is an academic project for the single purpose of learning about post-quantum crpyography.\
It is NOT guaranteed to be secure.

# Table of Contents
1. [Introduction](#introduction)
2. [Why do we need NTRU?](#why-do-we-need-ntru)
3. [How it works](#how-it-works)
4. [Instructions](#instructions)
5. [Upcoming Features/Known Bugs](#upcoming-features/known-bugs)
6. [Additional Reading](#additional-reading)

# Introduction

# Why do we need NTRU?
The world of modern cryptography faces a huge, imminent problem. For years, we have relied on public-key schemes such as
RSA, Diffe-Hellman and Elliptic-Curve cryptography to facilitate secure communication between people all over the world.
However, we are quickly approaching a turning point. The rise of a commercial quantum computer is becoming increasingly
likely, with quantum supremacy was achieved just last year. This poses a huge threat to these schemes.\
https://ai.googleblog.com/2019/10/quantum-supremacy-using-programmable.html

So why is this an issue? Well, each scheme relies on the computational hardness of a certain mathematical problems (such as finding
the discrete logarithm of a number in a prime group, or factorising the product of two primes) to ensure the cryptographic
security it provides is hard to break. Unfortunately, all of these schemes are derived from problems which are very hard to solve on
 conventional computers, yet are easily broken by a hypothetical quantum computer. This may not have been much of a concern 20 years ago, were a quantum computer was just
 a figment of imagination, but as we plan for the future it is becoming increasingly clear that for any long term security, schemes which are resiliant to both conventional and
 quantum computers need to be adopted.

Luckily, there are myriad mathematical problems which fit this description, with no efficient quantum algorithm to solve them.
The NTRU-Cryptosystem (comprised of NTRUEncrypt and NTRUSign) is an upcoming candidate for a post-quantum public-key cryptosystem, and utilises
the hardness of the "shortest vector problem" within lattices (a form of lattice-based cryptography) to provide quantum-resiliant communication.
# How it works
Now, lets send a message between Alice and Bob. In line with traditional public-key cryptography, Bob is required to generate a 
public key (visible to everyone) and a private key (surprisingly kept private).

We first start by generating two polynomials, f(x) and g(x), with coefficient in the set {-1, 0, 1} and with degree at most N - 1\
We also require that f is invertible mod p and mod q. Hence, there exists an Fp and Fq such that:

`f * Fp (mod p) == 1 && f * Fq (mod q) == 1`

We then take the tuple (f, Fp) to be Bob's private key\

We then generate his public key h as follows:

`h = pFq * g (mod q)`

Example with p = 3, q = 32, n = 11:

`F(X) = 1 + 0x^1 + -1x^2 + -1x^3 + 0x^4 + 0x^5 + 0x^6 + 1x^7 + 0x^8 + -1x^9`

`G(X) = 1 + -1x^1 + 0x^2 + 1x^3 + 0x^4 + 0x^5 + 1x^6 + 0x^7 + 0x^8 + 1x^9 + -1x^10`

Using the Euclidean algorithm for polynomials, we calculate the following inverses:

`Fp(X): 2 + 0x^1 + 0x^2 + 1x^3 + 1x^4 + 1x^5 + 1x^6 + 2x^7 + 0x^8 + 2x^9 + 1x^10 (mod 3)`

`Fq(X) = 28 + 25x^1 + 4x^2 + 2x^3 + 22x^4 + 1x^5 + 13x^6 + 20x^7 + 20x^8 + 25x^9 + 31x^10 (mod 32)`

We then calculate the public key as:

`H(X): 23 + 3x^1 + 22x^2 + 11x^3 + 22x^4 + 31x^5 + 30x^6 + 17x^7 + 1x^8 + 19x^9 + 7x^10`

# Upcoming Features/Known Bugs

# Additional Reading
https://en.wikipedia.org/wiki/NTRUEncrypt
https://blog.isec.pl/ntru-public-key-cryptosystem-explained/#:~:text=NTRU%20(NTRUEncrypt%20and%20NTRUSign)%20is,special%20points%20in%20a%20lattice.
https://web.wpi.edu/Pubs/ETD/Available/etd-0430102-111906/unrestricted/corourke.pdf
https://assets.onboardsecurity.com/static/downloads/NTRU/resources/NTRUTech014.pdf
https://stackoverflow.com/questions/2421409/algorithm-for-computing-the-inverse-of-a-polynomial/2426520#2426520
https://crypto.stackexchange.com/questions/78200/how-to-find-the-inverse-of-a-polynomial-in-ntru-pkcs