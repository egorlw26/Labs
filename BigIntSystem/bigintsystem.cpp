#include "bigintsystem.h"

BigIntSystem::BigIntSystem():
    condts(0), n_condts(0)
{}

BigIntSystem::BigIntSystem(int n):
    n_condts(n)
{
    for(int i = 0; i<n; i++)
    {
        BigIntMod bim;
        std::cin>>bim;
        condts.push_back(bim);
    }
}


BigIntSystem::BigIntSystem(std::vector<BigIntMod> vec):
    condts(vec), n_condts(vec.size())
{}

BigIntSystem::~BigIntSystem()
{}

bool BigIntSystem::hasSolution()
{
    for(int i = 0; i<condts.size()-1; i++)
        for(int j = i+1; j<condts.size(); j++)
        {
            BigInt gcd = condts[i].getMod().bi_gcd(condts[j].getMod());
            if((condts[i].getNumber() - condts[j].getNumber()).bi_mod(gcd) !=0)
                return false;
        }
        return true;
}

BigIntMod BigIntSystem::solve()
{
    if(!hasSolution())
        throw "This system doesn't have solution!";
    else
    {
       BigIntMod res = condts[0], right;
       for(int i = 1; i<n_condts; i++)
       {
           right = condts[i];
           //std::cout<<res<<std::endl;
           //std::cout<<right<<std::endl;
           BigInt number = res.getNumber(), mod = right.getMod();
           BigIntMod t(number, mod);
           //std::cout<<t<<std::endl;
           right = right - t;
           //std::cout<<right<<std::endl;
           number = res.getMod();
           t = BigIntMod(number, mod);
           //std::cout<<t<<std::endl;
           //std::cout<<t.inverse()<<std::endl;
           right = right * t.inverse();
           res.setNumber(res.getNumber() + res.getMod()*right.getNumber());
           res.setMod(res.getMod()*right.getMod());
           std::cout<<std::endl;
       }
       return res;
    }
}