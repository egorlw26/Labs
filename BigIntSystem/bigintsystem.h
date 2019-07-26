#ifndef CRYPTOGRAPHYLAB1_BIGINTSYSTEM_H
#define CRYPTOGRAPHYLAB1_BIGINTSYSTEM_H

#include "BigIntMod.h"
#include <vector>

class BigIntSystem
{
public:
    BigIntSystem();

    ~BigIntSystem();

    BigIntSystem(int n);

    BigIntSystem(std::vector<BigIntMod> vec);

    bool hasSolution();

    BigIntMod solve();

    friend std::ostream& operator << (std::ostream& os, const BigIntSystem bis)
    {

    }

    friend std::istream& operator >> (std::istream& is, BigIntSystem& bis)
    {
        int n;
        std::cin>>n;
        std::vector<BigIntMod> syst(n);
        for(int i = 0; i<n; i++)
        {
            BigIntMod bim;
            std::cin>>bim;
            syst[i] = bim;
        }
        bis = BigIntSystem(syst);
        return is;
    }


private:
    int n_condts;
    std::vector<BigIntMod> condts;
};


#endif //CRYPTOGRAPHYLAB1_BIGINTSYSTEM_H
