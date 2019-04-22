#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <map>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <dos.h>

#include "omp.h"

std::string info;
unsigned long long numOfWordsToCheck = 9 * 1e6;
time_t start;
std::ifstream fin("Salvatore.txt");
int ans = 0;
int numThreads;
int main()
{
	if (fin.is_open())
	{
		std::cout << "Open\n";
		std::string word1;
		while (fin >> word1)
			info += word1 + ' ';
		
		//straight
		
		char *line = new char[numOfWordsToCheck];
		for (int i = 0; i < numOfWordsToCheck; ++i)
			line[i] = info[i];
		std::cout << "STARTED" << std::endl;
		start = clock();
		char word[30];
		int wordLength;
		for (; std::sscanf(line, "%29s%n", word, &wordLength) == 1; line += wordLength)
		{
			std::string s(word);
			if (s == "Drizzt")
				ans++;
		}
		

		//OpenMP
		/*numThreads = 4;
		int portion = numOfWordsToCheck / numThreads;


		std::vector<std::string> vec;
		for (int i = 0; i < numThreads; ++i)
			vec.push_back(info.substr(i*portion, portion));

		std::cout << "STARTED" << std::endl;
		start = clock();
#pragma omp parallel for
		for (int i = 0; i < numThreads; ++i)
		{
			char *line = new char[vec[i].length()];
			for (int j = 0; j < vec[i].length(); ++j)
				line[j] = vec[i][j];

			char word[30];
			int wordLength;

			for (; std::sscanf(line, "%29s%n", word, &wordLength) == 1; line += wordLength)
			{
				std::string s(word);
				if (s == "Drizzt")
					ans++;
			}

		}
		*/
	}
	else return 0;

	std::cout << ans << std::endl;
		fin.close();
	std::cout << "\n\n\n";
	std::cout << (clock() - start) / CLOCKS_PER_SEC << " seconds\n";

	system("pause");
	return 0;
}