#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <random>

using namespace std;

struct dot
{
	double x;
	double y;
	int number;

	double operator[](int i) const 
	{
		return i ? y : x;
	}

	double& operator[](int i)
	{
		return i ? y : x;
	}

	bool operator == (const dot &o)
	{
		return x == o.x && y == o.y;
	}

	bool operator < (const dot &o)
	{
		return x < o.x || (x == o.x && y < o.y);
	}

};

void getDots(const vector<dot>& dots, ofstream &o)
{
	for (dot d : dots)
		o << d.x <<' '<<d.y<<endl;
}

double rotateZ(const dot &a, const dot &b, const dot &c) //find Z coordinat of cross product to find the rotation
{
	return (b[0] - a[0]) * (c[1] - b[1]) - (b[1] - a[1]) * (c[0] - b[0]);
}

vector<dot> grahamScan(vector<dot>& dots) //our first state to work with
{
	int size = dots.size();
	if (size < 3)
		return dots;

	vector<int> inds(size);
	for (int i = 0; i < size; ++i)
		inds[i] = i;

	int min_ind = 0;
	for (int i = 0; i < size; ++i)
		if (dots[i][0] < dots[min_ind][0])
			min_ind = i;
	
	int tInd = inds[0];
	inds[0] = inds[min_ind];
	inds[min_ind] = tInd;

	for (int i = 2; i < size; ++i)
	{
		int j = i;
		while (j > 1 && rotateZ(dots[inds[0]], dots[inds[j - 1]], dots[inds[j]]) < 0)
		{
			int tInd = inds[j - 1];
			inds[j - 1] = inds[j];
			inds[j] = tInd;
			j--;
		}
	}

	vector<int> tres;
	tres.push_back(inds[0]);
	tres.push_back(inds[1]);
	vector<dot> res;
	res.push_back(dots[inds[0]]);
	res.push_back(dots[inds[1]]);

	for (int i = 2; i < size; ++i)
	{
		while (rotateZ(dots[tres[tres.size() - 2]], dots[tres[tres.size() - 1]], dots[inds[i]]) < 0)
		{
			tres.pop_back();
			res.pop_back();
		}
		tres.push_back(inds[i]);
		res.push_back(dots[inds[i]]);
	}
	return res;
}

double ShoelaceFormula(vector<dot> dots) //to find area of our figure
{
	dots.push_back(dots[0]);
	double area = 0;
	for (int i = 0; i < dots.size() - 1; ++i)
		area += dots[i][0] * dots[i + 1][1] - dots[i][1] * dots[i + 1][0];
	return area / 2;
}

bool isTwoSegmentCross(const dot &a1, const dot &a2, const dot &b1, const dot &b2)
{
	double a = rotateZ(a1, a2, b1) * rotateZ(a1, a2, b2);
	double b = rotateZ(b1, b2, a1) * rotateZ(b1, b2, a2);
	return (a < 0 && b < 0);
}

bool isThereSelfCross(const dot &a, const dot &b, vector<dot> &dots)
{
	for (int i = 0; i < dots.size() - 1; ++i)
		if (isTwoSegmentCross(a, b, dots[i], dots[i + 1]))
			return true;
	return false;
}

bool isDotInside(const dot &t1, const dot &t2, const dot &t3, const dot &d)
{
	double r1 = rotateZ(t1, t2, d);
	double r2 = rotateZ(t2, t3, d);
	double r3 = rotateZ(t3, t1, d);
	return (r1 < 0 && r2 < 0 && r3 < 0);
}

bool isOneOfDotsInside(const dot &t1, const dot &t2, const dot &t3, vector<dot> &dots)
{
	for (dot d : dots)
		if (isDotInside(t1, t2, t3, d))
			return true;
	return false;
}

vector<dot> diving(vector<dot> &currentState, vector<dot> &freeDots)
{
	int indexFree = rand() % (freeDots.size());
	int indexCurrState = rand() % (currentState.size() - 1);
	if (
		isThereSelfCross(currentState[indexCurrState], freeDots[indexFree], currentState)
		|| isThereSelfCross(currentState[indexCurrState + 1], freeDots[indexFree], currentState)
		|| isOneOfDotsInside(currentState[indexCurrState], freeDots[indexFree], currentState[indexCurrState + 1], freeDots)
		)
		return vector<dot>(0);
	vector<dot> res = currentState;
	res.insert(res.begin() + indexCurrState + 1, freeDots[indexFree]);
	return res;
}

vector<dot> cuttingOf(vector<dot> &currentState)
{
	int index = rand() % (currentState.size() - 2) + 1;
	if (
		isThereSelfCross(
			currentState[index - 1], currentState[index + 1], currentState)
		||
		rotateZ(
			currentState[index - 1], currentState[index], currentState[index + 1]) > 0
		)
		return vector<dot>(0);
	auto res = currentState;
	res.erase(res.begin() + index);
	return res;
}

void decreaseTemperature(double& temp)
{
	temp -= 0.0001;
}

vector<dot> logic(vector<dot> dots)
{
	vector<dot> currentState = grahamScan(dots);
	vector<dot> ans = currentState;
	vector<dot> freeDots;
	for (dot d : dots)
		if (find(currentState.begin(), currentState.end(), d) == currentState.end())
			freeDots.push_back(d);
	double T = 100;
	double tEnd = 0;
	vector<dot> tState;
	while (T > tEnd)
	{
		bool br = false;
		if (((double)rand() / (RAND_MAX)) < 0.9 && freeDots.size() != 0)
		{
			while (true)
			{
				tState = diving(currentState, freeDots);
				if (!tState.empty())
					break;
				else
				{
					br = true;
					break;
				}
			}
		}
		else
		{
			while (true)
			{
				tState = cuttingOf(currentState);
				if (!tState.empty())
					break;
				else
				{
					br = true;
					break;
				}
			}
		}
		if (br)
			continue;

		double dE = ShoelaceFormula(currentState) - ShoelaceFormula(tState);
		if (dE > 0)
		{
			currentState = tState;
			freeDots.clear();
			for (dot d : dots)
				if (find(currentState.begin(), currentState.end(), d) == currentState.end())
					freeDots.push_back(d);
		}
		else
		{
			if ((rand() % (RAND_MAX))*T > -dE)
			{
				currentState = tState;
				freeDots.clear();
				for (dot d : dots)
					if (find(currentState.begin(), currentState.end(), d) == currentState.end())
						freeDots.push_back(d);
			}
		}
		if (ShoelaceFormula(ans) > ShoelaceFormula(currentState))
			ans = currentState;
		decreaseTemperature(T);
	}
	ans.push_back(ans[0]);
	return ans;
}

vector<dot> generateDots(int size)
{
	vector<dot> res;
	for (int i = 0; i < size; ++i)
	{
		double x, y;
		x = ((double)rand() / (RAND_MAX))*100;
		y = ((double)rand() / (RAND_MAX))*100;
		res.push_back(dot{ x, y, i});
	}
	return res;
}

int main()
{
	srand(time(NULL));
	time_t start = clock();
	vector<dot> arra = generateDots(100);
	cout << "Generated\n";
	vector<dot> ans = logic(arra);
	cout << "Work Done\n";
	cout << (clock() - start)*1000 / CLOCKS_PER_SEC << endl;

	//in the end i write my results to txt files and then open them in python for visualization. In future i'll add here OpenGL.
	ofstream out;
	out.open("out1.txt");
	getDots(ans, out);
	out.close();
	out.open("out2.txt");
	getDots(arra, out);
	out.close();


	system("pause");
	return 0;
}