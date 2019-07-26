#ifndef BIGINT_H
#define BIGINT_H
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class BigInt {
public:
    BigInt();

    ~BigInt();

    BigInt(long long x);

    BigInt(std::string s);

    BigInt(const BigInt &other);

    BigInt sum(const BigInt &other) const;

    BigInt subtract(const BigInt &other) const;

    BigInt multiply(const BigInt &other) const;

    BigInt divide(const BigInt &other) const;

    BigInt div2() const;

    BigInt power(const BigInt &other) const;

    BigInt bi_sqrt() const;

    BigInt mod2() const;

    BigInt bi_mod(const BigInt &other) const;

    static BigInt bi_eegcd(BigInt a, BigInt b, BigInt &x, BigInt &y) const;

    static BigInt abs(const BigInt &bi)
    {
        BigInt result(bi);
        result.m_sgn = 1;
        return result;
    }

    BigInt max(const BigInt &left, const BigInt &right) const;

    BigInt min(const BigInt &left, const BigInt &right) const;

    BigInt bi_gcd(const BigInt& other) const;

    BigInt operator-() const;

    BigInt operator = (const BigInt &other);

    bool operator == (const BigInt &other) const;

    bool operator != (const BigInt& other) const;

    bool operator < (const BigInt &other) const;

    bool operator <= (const BigInt &other) const;

    bool operator > (const BigInt &other) const;

    bool operator >= (const BigInt &other) const;


    friend BigInt operator+(const BigInt &left, const BigInt &right) {
        return left.sum(right);
    }

    friend BigInt operator-(const BigInt &left, const BigInt &right) {
        return left.subtract(right);
    }

    friend BigInt operator*(const BigInt &left, const BigInt &right) {
        return left.multiply(right);
    }

    friend BigInt operator/(const BigInt &left, const BigInt &right) {
        return left.divide(right);
    }

    friend BigInt operator ^(const BigInt& left, const BigInt& right)
    {
        return left.power(right);
    }

    friend BigInt operator % (const BigInt& left, const BigInt& right)
    {
        return left.bi_mod(right);
    }

    friend std::ostream& operator <<(std::ostream& os, const BigInt& bi)
    {
	    os<<bi.m_numbers[0]*bi.m_sgn;
        for(unsigned int i = 1; i<bi.m_numbers.size(); i++)
            os<<bi.m_numbers[i];
        return os;
    }

    friend std::istream& operator >>(std::istream& is, BigInt& bi)
    {
        std::string s;
        is>>s;
        bi = BigInt(s);
        return is;
    }

private:
    int m_lenght;
    std::vector<int> m_numbers;
	int m_sgn;
};

#endif // BIGINT_H
