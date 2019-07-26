#include "BigIntMod.h"

BigIntMod::BigIntMod():
    m_number(0), m_mod(0)
{}

BigIntMod::~BigIntMod()
{}

BigIntMod::BigIntMod(BigInt &n, BigInt &m):
    m_number(n.bi_mod(m)), m_mod(m)
{}

BigIntMod::BigIntMod(const BigIntMod &other):
    m_number(other.m_number), m_mod(other.m_mod)
{}

BigIntMod BigIntMod::sum(const BigIntMod& other) const
{

    if(m_mod == other.m_mod)
    {
        BigIntMod result(*this);
        result.m_number = (m_mod + (result.m_number + other.m_number).bi_mod(m_mod)).bi_mod(m_mod);
        return result;
    }
    else
    {
        throw "Nihatit";
    }

}

BigIntMod BigIntMod::subtract(const BigIntMod &other) const
{
    if(m_mod == other.m_mod)
    {
        BigIntMod result;
        result.m_number = (m_number - other.m_number).bi_mod(m_mod);
        result.m_mod = m_mod;
        return result;
    }
    else
    {
        throw "Shot ne tak";
    }
}

BigIntMod BigIntMod::multiply(const BigIntMod &other) const
{
    if(m_mod==other.m_mod)
    {
        BigIntMod result;
        result.m_number = (m_number*other.m_number).bi_mod(m_mod);
        result.m_mod = m_mod;
        return result;
    }
}

BigIntMod BigIntMod::divide(const BigIntMod &other) const
{
    return((*this)*other.inverse());
}

BigIntMod BigIntMod::inverse() const
{
    BigIntMod x;
    BigInt y;
    BigInt g = g.bi_eegcd(m_number, m_mod, x.m_number, y);
    if (g !=1)
        throw "No inverse element!";
    else
    {
        x.m_number = (x.m_number%m_mod + m_mod)%m_mod;
        x.m_mod = m_mod;
        return x;
    }
}