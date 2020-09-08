# SVCrypt
C++ implementation of NTRU Cryptosystem\
https://en.wikipedia.org/wiki/NTRUEncrypt

**WARNING: Do not use this to encrypt anything**\
It is an academic project for the single purpose of learning about post-quantum crpyography.\
It is NOT guaranteed to be secure.

# Table of Contents
1. [Why do we need NTRU?](#why-do-we-need-ntru)
2. [How it works](#how-it-works)
3. [Instructions](#instructions)
4. [Additional Reading](#additional-reading)

# Why do we need NTRU?
The world of modern cryptography faces a huge, imminent problem. For years, we have relied on public-key schemes such as
RSA, Diffe-Hellman and Elliptic-Curve cryptography to facilitate secure communication between people all over the world.
However, we are quickly approaching a turning point. The rise of a commercial quantum computer is becoming increasingly
likely, with quantum supremacy was achieved just last year. This poses a huge threat to these schemes.\
https://ai.googleblog.com/2019/10/quantum-supremacy-using-programmable.html\
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
Now, lets send a message between Alice and Bob. In line with traditional public-key cryptography, each user is required to generate a 
public key (visible to everyone) and a private key (surprisingly kept private).