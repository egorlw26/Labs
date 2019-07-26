#include <iostream>
#include <algorithm>
#include <string>
#include <math.h>
#include <map>
#include <chrono>
#include "bigint.h"
#include "BigIntMod.h"
#include "bigintsystem.h"



bool MillerRabin(BigInt n, BigInt k);

BigInt rand(const BigInt& N)
{
	srand(time(NULL));
	int l = N.getBILength();
	BigInt res = 0;
	for (int i = 0; i < l + 2; i++)
	{
		int r = rand() % 10;
		res = res + r * pow(10, i);
	}
	return res % N;
}

BigInt gxRho(BigInt x, BigInt mod)
{
	//x = x % mod;
	return (x*x + 1) % mod;
}

BigInt RhoPollandF(const BigInt& N)
{
	for (BigInt i = 2; i < 100; i = i + 1)
		if (N%i == 0)
			return i;
	BigInt x = 2, y = 2, d = 1;
	while (d == 1)
	{
		x = gxRho(x, N);
		y = gxRho(gxRho(y, N), N);
		d = N.bi_gcd(BigInt::abs(x - y));
	}

	return d;
}

std::pair<BigInt, BigInt> whatPower(BigInt number)
{
	if (number == 2 || number == 3)
		return std::pair<BigInt, BigInt>(number, 1);
	if (MillerRabin(number, number.getBILength()))
		return std::pair<BigInt, BigInt>(number, 1);
	for (BigInt i = 2; i <= number.bi_sqrt(); i = i + 1)
	{
		BigInt counter = 0;
		BigInt t = number;
		while (t%i == 0)
		{
			counter = counter + 1;
			t = t / i;
		}
		if (t == 1)
			return std::pair<BigInt, BigInt>(i, counter);
	}
	return std::pair<BigInt, BigInt>(BigInt(-1), BigInt(0));
}

std::vector<std::pair<BigInt, BigInt>> Factorize(BigInt number)
{
	std::vector<std::pair<BigInt, BigInt>> ans;
	ans.push_back(std::pair<BigInt, BigInt>(1, 0));
	bool p = false;
	while (number != 1)
	{
		BigInt d = RhoPollandF(number);
		std::pair<BigInt, BigInt> temp = whatPower(d);
		number = number / d;

		while (number % temp.first == 0)
		{
			temp.second = temp.second + 1;
			number = number / temp.first;
		}
		if (!p)
		{
			p = true;
			ans[0] = temp;
		}
		else
			ans.push_back(temp);
		if (MillerRabin(number, number.getBILength()))
		{
			ans.push_back(std::pair<BigInt, BigInt>(number, 1));
			return ans;
		}
	}
	return ans;
}

BigInt BabyStepGiantStep(const BigInt& a, const BigInt& b, const BigInt& m)
{
	if (a.bi_gcd(m) != 1)
	{
		std::cout << "Wrong input" << std::endl;
		return -1;
	}
	BigInt n = m.bi_sqrt() + 1;
	std::map<BigInt, BigInt> vals;
	for (BigInt i = n; i >= 1; i = i - 1)
		vals[a.powerMod(i*n, m)] = i;
	for (BigInt i = 0; i <= n; i = i + 1)
	{
		BigInt current = (a.powerMod(i, m)*b) % m;
		if (vals.count(current))
		{
			BigInt ans = vals[current] * n - i;
			if (ans < m)
				return  ans;
		}
	}
	return -1;
}

BigInt EulerFunc(BigInt n)
{
	BigInt result = n;
	for (BigInt i = 2; i*i <= n; i = i + 1)
	{
		if (n%i == 0)
		{
			while (n%i == 0)
				n = n / i;
			result = result - result / i;
		}
	}
	if (n > 1)
		result = result - result / n;
	return result;
}

int MobiusFunc(BigInt n)
{
	BigInt count = 0;
	BigInt s = n.bi_sqrt();
	for (BigInt i = 2; i <= s; i = i + 1)
	{
		int zer = 0;
		while (n%i == 0)
		{
			n = n / i;
			count = count + 1;
			zer++;
			if (zer >= 2)
				return 0;
		}
	}
	if (n > 1)
		count = count + 1;
	if (count % 2 == 1)
		return -1;
	return 1;
}

int solveLegendre(BigInt a, BigInt p)
{
	if (a%p == 0)
		return 0;
	if (a == 1)
		return 1;
	for (BigInt i = 2; i < p; i = i + 1)
	{
		if ((i*i) % p == a)
			return 1;
	}
	return -1;
}

int solveJacobi(BigInt a, BigInt p)
{
	if (p % 2 == 0)
		return -1;
	std::vector<BigInt> factors;
	for (BigInt i = 2; i <= p.bi_sqrt(); i = i + 1)
	{
		while (p%i == 0)
		{
			factors.push_back(i);
			p = p / i;
		}
	}
	int ans = 1;
	for (int i = 0; i < factors.size(); i++)
		ans *= solveJacobi(a, factors[i]);
	return ans;
}

bool MillerRabin(BigInt n, BigInt k)
{
	if (n == 1)
		return false;
	if (n == 2 || n == 3 || n == 5)
		return true;
	if (n % 2 == 0)
	{
		//		std::cout << "Wrong N" << std::endl;
		return 0;
	}
	BigInt d = n - 1;
	BigInt s = 0;
	while (d % 2 == 0)
	{
		s = s + 1;
		d = d / 2;
	}
	for (BigInt i = 0; i < k; i = i + 1)
	{
		bool p = false;
		BigInt a = rand(n - 2) + 2;
		BigInt x = a.powerMod(d, n);
		if (x == 1 || x == n - 1)
			continue;
		for (BigInt j = 0; j < s; j = j + 1)
		{
			x = x.powerMod(2, n);
			if (x == n - 1)
			{
				p = true;
				break;
			}
		}
		if (p)
			continue;
		return false;
	}
	return true;
}

BigInt CipollasAlgorithm(BigInt n, BigInt p)
{
	if (!MillerRabin(p, p.getBILength()))
	{
		std::cout << "Wrong p!\n";
		return -1;
	}
	n = n % p;
	if (solveLegendre(n, p) != 1)
		return -1;
	BigInt a;
	for (BigInt i = 1; i < p; i = i + 1)
	{
		BigInt t = (i*i - n) % p;
		if (solveLegendre(t, p) == -1)
		{
			a = i;
			break;
		}
	}
	BigInt result = 0;
	BigInt b = a * a - n;
	BigInt s = (p + 1) / 2;

	for (BigInt i = 0; i <= s; i = i + 2)
	{
		result = (result + (s.factorial() / (i.factorial()*(s - i).factorial()))*a.power(s - i)*b.power(i / 2)) % p;
	}
	return result;
}

BigInt randomSimple()
{
	//return BigInt("64376629");
	BigInt temp = rand(BigInt("100000000")) + BigInt("2000000000");
	while (!MillerRabin(temp, temp.getBILength()))
	{
		temp = temp - 1;
	}
	return temp;
}

BigInt findPrimitiveRootModule(BigInt number)
{
	std::vector<std::pair<BigInt, BigInt>> factors = Factorize(number - 1);
	for (BigInt i = 2; i < number; i = i + 1)
	{
		bool p = false;
		for (std::pair<BigInt, BigInt> j : factors)
			if (i.powerMod((number - 1) / j.first, number) == 1)
				p = true;
		if (p)
			continue;
		return i;
	}
	return 1;
}

std::pair<BigInt, BigInt> ElGamal()
{
	BigInt p = randomSimple();
	std::cout << "Random Prime = " << p << std::endl;
	BigInt g = findPrimitiveRootModule(p);
	std::cout << "Primitive Root Module = " << g << std::endl;
	std::cout << "Generating closed key" << std::endl;
	BigInt x = rand(p - 2) + 1;
	std::cout << "Generating open key" << std::endl;
	BigInt y = g.powerMod(x, p);
	std::cout << "Closed key = " << x << ", Open key = " << y << std::endl;
	
	bool t = true;
	std::cout << "Enter your message: \n";
	BigInt message;
	while (t)
	{
		std::cin >> message;
		if (message < p)
			t = false;
		else
			std::cout << "Wrong message size, try again: \n";
	}
	std::cout << "Genering random session key: \n" << std::endl;
	BigInt k = rand(p - 1) + 1;
	std::cout << "Session key = " << k << std::endl;
	std::pair<BigInt, BigInt> ans;
	ans.first = g.powerMod(k, p);
	ans.second = y.powerMod(k, p);

	return ans;
}

void Interface()
{
	bool p = true;
	int choice;
	while (p)
	{
		std::cout << "Choose, what you want to do: \n"
			"	1 - factorize with rho-Polland algoritgm \n"
			"	2 - find logarithm with Baby-Step-Giant-Step algorithm \n"
			"	3 - solve Euler function \n"
			"	4 - solve Mobius function \n"
			"	5 - solve Legendre symbol \n"
			"	6 - solve Jacobi symbol \n"
			"	7 - find sqrt with Cipolla algorithm \n"
			"	8 - check if Prime with Miller-Rabin algorithm \n"
			"	9 - start El-Gamal \n";
		std::cin >> choice;
		if (choice <= 0 || choice >= 10)
		{
			std::cout << "		Wrong variant, try again!" << std::endl;
			continue;
		}
		p = false;
	}

	if(choice ==  1)
	{
		std::cout << "Enter number: \n";
		BigInt number;
		std::cin >> number;
		std::cout << "Result of factorization is:\n";
		std::vector<std::pair<BigInt, BigInt>> factors = Factorize(number);
		for (std::pair<BigInt, BigInt> pair : factors)
			std::cout << pair.first << '^' << pair.second << ' ';
		std::cout << std::endl;
	}
	if( choice == 2)
	{
		std::cout << "Enter base, number and mod. Keep in mind, that GCD(base, mod) == 1: \n";
		BigInt base, number, mod;
		std::cin >> base>> number>> mod;
		std::cout << "Result of BSGS: \n";
		std::cout << BabyStepGiantStep(base, number, mod) << std::endl;
	}
	if (choice == 3)
	{
		std::cout << "Enter number: \n";
		BigInt number;
		std::cin >> number;
		std::cout << "Result of Euler function = \n";
		std::cout << EulerFunc(number) << std::endl;
	}
	if (choice == 4)
	{
		std::cout << "Enter number: \n";
		BigInt number;
		std::cin >> number;
		std::cout << "Result of Mobius function = \n";
		std::cout << MobiusFunc(number) << std::endl;
	}
	if (choice == 5)
	{
		std::cout << "Enter upper number and prime number:\n";
		BigInt a, p;
		std::cin >> a >> p;
		std::cout << "Legendre for this pair = \n" << solveLegendre(a, p) << std::endl;
	}
	if (choice == 6)
	{
		std::cout << "Enter upper number and prime number:\n";
		BigInt a, p;
		std::cin >> a >> p;
		std::cout << "Jacobi for this pair = \n" << solveJacobi(a, p) << std::endl;
	}
	if (choice == 7)
	{
		std::cout << "Enter number and mod (mod must be prime): \n";
		BigInt number, mod;
		std::cin >> number>>mod;
		std::cout << "Cipollas answer's = \n";
		std::cout << CipollasAlgorithm(number, mod) << std::endl;
	}
	if (choice == 8)
	{
		std::cout << "Enter number and iterations: \n";
		BigInt number, it;
		std::cin >> number>>it;
		std::cout << "Result of Miller-Rabin algorithm = \n";
		std::cout << MillerRabin(number, it) << std::endl;
	}
	if (choice == 9)
	{
		std::cout << "Just sit and wait, it can take a while... \n";
		auto gamal = ElGamal();
		std::cout << "ElGamal pair: \n"
			"a = " << gamal.first << " "
			"b = " << gamal.second << std::endl;
	}

}


int main()
{
	Interface();
	system("pause");
	return 0;
}
//64376629
//2031543611