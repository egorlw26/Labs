#include "bigint.h"

namespace
{
	unsigned int getLength(int x)
	{
        if(x==0)
            return 1;
		int result = 0;
		while (std::abs(x) > 0)
		{
			result++;
			x /= 10;
		}
		return result;
	}
}

BigInt::BigInt() :
	m_lenght(0),
	m_numbers(),
	m_sgn(1)
{
}

BigInt::~BigInt()
{

}


BigInt::BigInt(long long x) :
	m_lenght(getLength(x)), m_sgn(1)
{
	m_numbers.resize(m_lenght);
	if (x < 0)
		m_sgn = -1;
	x = std::abs(x);
	for (int i = m_numbers.size() - 1; i >= 0; i--)
	{
		m_numbers[i] = x % 10;
		x /= 10;
	}
}

BigInt::BigInt(std::string s) :
	m_lenght(s.length()), m_sgn(1)
{
	if (s[0] == '-')
	{
		m_lenght--;
		m_sgn = -1;
		s = s.substr(1, s.length() - 1);
	}
	m_numbers.resize(m_lenght);
	for (int i = 0; i < m_numbers.size(); i++)
	{
		m_numbers[i] = s[i] - '0';
	}
 }

BigInt::BigInt(const BigInt &other)
{
	m_lenght = other.m_lenght;
	m_numbers = other.m_numbers;
	m_sgn = other.m_sgn;
}

BigInt BigInt::sum(const BigInt &other) const
{
	BigInt result;
	result.m_lenght = std::max(m_lenght, other.m_lenght)+1;
	result.m_numbers.resize(result.m_lenght);
	BigInt M = max(abs(*this), abs(other));
    for(int i = 0; i < M.m_lenght; i++)
        result.m_numbers[result.m_lenght-1-i] = M.m_numbers[M.m_lenght - 1 - i];

    BigInt m = min(abs(*this), abs(other));

    if(m_sgn*other.m_sgn == 1)
    {
        for(int i = 0; i<std::min(m_lenght, other.m_lenght); i++)
        {
            int t = result.m_numbers[result.m_lenght - 1 - i] + m.m_numbers[m.m_lenght - i - 1];
            result.m_numbers[result.m_lenght-1-i] = t%10;
            if(t>=10)
                result.m_numbers[result.m_lenght-i-2] ++;
        }

        for(int i = 0; i<M.m_lenght; i++)
        {
        	if(result.m_numbers[result.m_lenght - 1 - i] >=10)
			{
        		result.m_numbers[result.m_lenght-1-i]%=10;
        		result.m_numbers[result.m_lenght-2-i]++;
			}
		}

        result.m_sgn = m_sgn;
    }
    else {
        if (m_sgn == -1)
        {
            result = other.subtract(-*this);
        }
        else
        {
            result = subtract(-other);
        }
    }

    while(result.m_numbers[0] == 0 && result.m_lenght > 1)
    {
        result.m_numbers.erase(result.m_numbers.begin());
        result.m_lenght--;
    }

	return result;
}

BigInt BigInt::subtract(const BigInt& other) const
{
	BigInt result;
	BigInt M = max(abs(*this), abs(other));
	BigInt m = min(abs(*this), abs(other));
	result = M;
    if(m_sgn*other.m_sgn==  1)
    {
        for(int i = 0; i<std::min(m_lenght, other.m_lenght); i++)
        {
            int t = result.m_numbers[result.m_lenght-1-i] - m.m_numbers[m.m_lenght-1-i];
            result.m_numbers[result.m_lenght-1-i] = (t+10)%10;
            if(t<0)
                result.m_numbers[result.m_lenght-2-i]--;
        }

        for(int i =0; i<M.m_lenght; i++)
            if(result.m_numbers[result.m_lenght-1-i] <0)
            {
                result.m_numbers[result.m_lenght-1-i]+=10;
                result.m_numbers[result.m_lenght-2-i]--;
            }

            if(M.m_numbers == m_numbers)
                result.m_sgn = m_sgn;
            else
                result.m_sgn = -other.m_sgn;
    }
    else
    {
        if(m_sgn == -1)
        {
            result = other.sum(-*this);
            result.m_sgn = m_sgn;
        }

        else
        {
            result = sum(-other);
            result.m_sgn = m_sgn;
        }
    }

    while(result.m_numbers[0] == 0 && result.m_lenght>1)
    {
        result.m_numbers.erase(result.m_numbers.begin());
        result.m_lenght--;
    }

    return result;
}

BigInt BigInt::multiply(const BigInt &other) const {
    BigInt result = 0;
    result.m_lenght = m_lenght + other.m_lenght + 1;
    result.m_numbers.resize(result.m_lenght);
    for (int i = 0; i < other.m_lenght; i++) {
        for (int j = 0; j < m_lenght; j++) {
            int t = m_numbers[m_lenght - 1 - j] * other.m_numbers[other.m_lenght - 1 - i];
            result.m_numbers[result.m_lenght - 1 - i - j] += t % 10;
            if (t >= 10) {
                result.m_numbers[result.m_lenght - 2 - i - j] += t / 10;
            }
        }
    }

    for (int i = 0; i < result.m_lenght; i++) {
        if (result.m_numbers[result.m_lenght - 1 - i] >= 10) {
            result.m_numbers[result.m_lenght - 2 - i] += result.m_numbers[result.m_lenght - 1 - i] / 10;
            result.m_numbers[result.m_lenght - 1 - i] %= 10;
        }
    }

    while (result.m_numbers[0] == 0 && result.m_lenght > 1)
    {
        result.m_numbers.erase(result.m_numbers.begin());
        result.m_lenght--;
    }
    result.m_sgn = m_sgn*other.m_sgn;

    return result;
}

BigInt BigInt::divide(const BigInt &other) const
{
    BigInt result;
    if(abs(*this) < abs(other))
    {
        result = 0;
        return  result;
    }
    if(other == 1)
        return *this;
    if(other == 2 || other == -2)
        return div2()*other.m_sgn;
    BigInt a = 1;
    BigInt b = abs(*this);
    BigInt c=1;
    while(abs(abs(*this) - c*abs(other))>= abs(other))
    {
        c = (a+b).div2();
        if(abs(*this)-c*abs(other)<0)
            b = c;
        if(abs(*this) - c*abs(other)>abs(other))
            a = c;

    }

    result = c;
    result.m_sgn = m_sgn*other.m_sgn;
    if(*this - c*other >= 0 && *this - c*other<other)
    return result;
    else return  result-1;
}

BigInt BigInt::div2() const
{
    BigInt result;
    int prevMod = 0;
    for(int i = 0; i<m_lenght; i++)
    {
        if(m_numbers[i] < 2)
        {
            result.m_numbers.push_back(0+prevMod*5);
            if(i<m_lenght-1)
            {
                result.m_numbers.push_back((m_numbers[i] * 10 + m_numbers[i + 1]) / 2);
                result.m_lenght += 2;
            }
            else
            {
             result.m_lenght++;
            }
            prevMod = (m_numbers[i]*10+m_numbers[i+1])%2;
            i+=1;
        }
        else
        {
            result.m_numbers.push_back((m_numbers[i] + prevMod*10)/2);
            result.m_lenght++;
            prevMod = m_numbers[i]%2;
        }
    }

    result.m_sgn = m_sgn;

    while(result.m_numbers[0] == 0 && result.m_lenght>1)
    {
        result.m_lenght--;
        result.m_numbers.erase(result.m_numbers.begin());
    }

    return result;
}

BigInt BigInt::power(const BigInt &other) const
{
    if(other == 0)
        return 1;
    if(other<0)
        return 0;
    if(other.mod2()==1)
        return power(other-1)*(*this);
    else
    {
        BigInt b = power(other/2);
        return b*b;
    }
}

BigInt BigInt::bi_sqrt() const
{
    if(*this == 1)
        return 1;
    if(*this == 0)
        return 0;
    BigInt a = 1;
    BigInt b = *this+1;
    BigInt c = 1;
    while(1)
    {
        c = (a+b)/2;
        if(c*c<=*this && (c+1)*(c+1)>=*this)
            break;
        if(c*c>=*this)
            b = c;
        if(c*c<=*this && (c+1)*(c+1)<=*this)
            a = c;
    }
    return c;
}

BigInt BigInt::mod2() const
{
    return (*this - div2()*2 == 1);
}

BigInt BigInt::bi_mod(const BigInt &other) const
{
    if(*this>=0)
        return (*this - other*(*this/other));
    return (other - abs(*this).bi_mod(other));
}

static BigInt BigInt::bi_eegcd( BigInt a, BigInt b, BigInt &x, BigInt &y) const
{
    if(a==0)
    {
        x = 0; y = 1;
        return b;
    }
    BigInt x1, y1;
    BigInt d = bi_eegcd(b%a, a, x1, y1);
    x = y1 - (b/a)*x1;
    y = x1;
    return d;
}


BigInt BigInt::max(const BigInt &left, const BigInt &right) const
{
	if(left >= right)
		return left;
	return right;
}

BigInt BigInt::min(const BigInt &left, const BigInt &right) const
{
	if(left<=right)
		return left;
	return right;
}

BigInt BigInt::bi_gcd(const BigInt &other) const
{
    BigInt a(*this), b(other);
    while(a>0 && b>0)
    {
        if(a>b)
            a = a%b;
        else
            b = b%a;
    }
    return a+b;
}

BigInt BigInt::operator-() const
{
    BigInt res = *this;
    res.m_sgn = -m_sgn;
    return res;
}

BigInt BigInt::operator =(const BigInt& other)
{
	m_lenght = other.m_lenght;
	m_numbers = other.m_numbers;
	m_sgn = other.m_sgn;
	return *this;
}

bool BigInt::operator ==(const BigInt& other) const
{
	return (m_numbers == other.m_numbers && m_sgn == other.m_sgn);
}

bool BigInt::operator!=(const BigInt &other) const
{
    return !(*this == other);
}

bool BigInt::operator <(const BigInt& other) const
{
    if(*this == 0)
        return (other.m_sgn == 1);
    if(other == 0)
        return (m_sgn != 1);
    if(m_lenght*m_sgn < other.m_lenght * other.m_sgn)
        return true;
    if(m_lenght*m_sgn > other.m_lenght * other.m_sgn)
        return false;
    for(int i = 0; i<m_lenght; i++)
    {
        if(m_numbers[i]*m_sgn < other.m_numbers[i]*other.m_sgn)
            return true;
        if(m_numbers[i]*m_sgn > other.m_numbers[i]*other.m_sgn)
            return false;
    }

    return false;
}

bool BigInt::operator <=(const BigInt& other) const
{
	return (*this < other || *this == other);
}

bool BigInt::operator > (const BigInt& other) const
{
	return !(*this < other);
}

bool BigInt::operator >=(const BigInt& other) const
{
	return (*this > other || *this == other);
}