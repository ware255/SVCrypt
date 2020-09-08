#include "Polynomial.h"
#include "Parameters.h"
class Keys
{
    private:
        Polynomial publicKey;
        Polynomial privateKeyF;
        Polynomial privateKeyG;
        Polynomial privateKeyFp;
        Polynomial privateKeyFq;
    public:
        Keys(Parameters params);
        Polynomial getPublicKey();
        Polynomial getPrivateKeyF();
        Polynomial getPrivateKeyG();
        Polynomial getPrivateKeyFp();
        Polynomial getPrivateKeyFq();
};