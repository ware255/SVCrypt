#include <iostream>
#include "NTRU.h"
using namespace NTRU;

int main() {
    Parameters params(3,2,5,32,11);
    Keys keys(params);
    std::cout << keys.toString() << std::endl;

    std::string in = "Hello world, how are you? I'm Fine!";

    std::vector<Polynomial> enc = encryptString(in, keys.getPublicKey(), params);
    std::cout << "Encrypted" << Math::polysToBinaryString(enc, params.getN()) << std::endl;

    std::string dec = decryptString(enc, keys.getPrivateKeyF(), keys.getPrivateKeyFp(), params);
    std::cout << dec << std::endl;

    return 0;
}
