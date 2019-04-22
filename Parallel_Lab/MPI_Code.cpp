#include <stdio.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <conio.h>
#include <time.h>
#include <mpi.h>

std::ifstream fin("Salvatore.txt");
unsigned int numOfWordsToCheck = 9*1e6;
int portion, numOfProc, id;
std::string info;
MPI_Status status;
std::map<std::string, int> map;
const static int tag = 1;
time_t start;

int main(int argc, char** argv) {

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numOfProc);

	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	if (id == 0)
	{
		if (fin.is_open())
		{
			std::string word;
			while (fin>>word)
			{
				info += word + ' ';
			}
		}
		portion = numOfWordsToCheck / (numOfProc-1);
		const char *cInfo = info.c_str();
		std::cout << "STARTED" << std::endl;
		start = clock();
		for (int p = 1; p < numOfProc; ++p)
		{
			MPI_Send(&portion, 1, MPI_INT, p, tag, MPI_COMM_WORLD);
			MPI_Send(&cInfo[(p - 1)*portion], portion, MPI_CHAR, p, tag, MPI_COMM_WORLD);
		}
	}
	else
	{
		int rec;
		MPI_Recv(&rec, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

		char *line = new char[rec];
		MPI_Recv(line, rec, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
		std::cout << id << " started!" << " rec: "<<rec<< std::endl;
		time_t start = clock();
		int ans = 0;
		char word[30];
		unsigned wordLength;

		for (; std::sscanf(line, "%29s%n", word, &wordLength) == 1; line += wordLength)
		{
			std::string s(word);
			if (s == "Drizzt")
				ans++;
		}

		std::cout << "Id: " << id << " Info: " << ans <<" Time:"<< (clock() - start)/CLOCKS_PER_SEC<< std::endl;

		MPI_Send(&ans, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}

	if (id == 0)
	{
		int ans = 0;
		for (int p = 1; p < numOfProc; ++p)
		{
			int rec;
			MPI_Recv(&rec, 1, MPI_INT, p, tag, MPI_COMM_WORLD, &status);
			ans += rec;
		}

		std::cout << "Word found " << ans << " times. Elapsed time: "<<(clock()-start)/CLOCKS_PER_SEC<<" seconds." << std::endl;

	}


	MPI_Finalize();
	return 0;
}