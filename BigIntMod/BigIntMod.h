#ifndef CRYPTOGRAPHYLAB1_BIGINTMOD_H
#define CRYPTOGRAPHYLAB1_BIGINTMOD_H
#include "bigint.h"

class BigIntMod {
public:
    BigIntMod();

    ~BigIntMod();

    BigIntMod(BigInt& n, BigInt& m);

    BigIntMod(const BigIntMod& other);

    BigIntMod sum(const BigIntMod& other) const;

    BigIntMod subtract (const BigIntMod& other) const ;

    BigIntMod multiply (const BigIntMod& other) const ;

    BigIntMod divide (const BigIntMod& other) const;

    BigIntMod inverse() const;

    BigInt getNumber()
    {
        return m_number;
    }

    BigInt getMod()
    {
        return m_mod;
    }

    void setNumber (BigInt number)
    {
        m_number = number;
    }

    void setMod(BigInt mod)
    {
        m_mod = mod;
    }

    friend std::ostream& operator << (std::ostream& os, const BigIntMod& bim)
    {
        os<<bim.m_number<<" ("<<bim.m_mod<<')';
        return os;
    }

    friend std::istream& operator >> (std::istream& is, BigIntMod& bim)
    {
        BigInt c, m;
        is>>c>>m;
        bim = BigIntMod(c, m);
        return is;
    }

    friend BigIntMod operator - (const BigIntMod& left, const BigIntMod& right)
    {
        return left.subtract(right);
    }

    friend BigIntMod operator + (const BigIntMod& left, const BigIntMod& right)
    {
        return left.sum(right);
    }

    friend BigIntMod operator * (const BigIntMod& left, const BigIntMod& right)
    {
        return left.multiply(right);
    }

    friend BigIntMod operator / (const BigIntMod& left, const BigIntMod& right)
    {
        return left.divide(right);
    }



private:
    BigInt m_number;
    BigInt m_mod;

};


#endif //CRYPTOGRAPHYLAB1_BIGINTMOD_H
