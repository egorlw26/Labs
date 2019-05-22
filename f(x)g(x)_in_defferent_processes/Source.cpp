#include <iostream>
#include <math.h>
#include <thread>
#include <time.h>
#include <chrono>
#include <string>
#include <mpi.h>

int id, numOfProc;
const static int tag = 1;
bool rec1 = true;
bool rec2 = true;
MPI_Status status;
time_t fProcess = NULL, sProcess = NULL;


int fx(int x)
{
	std::this_thread::sleep_for(std::chrono::seconds(15));
	return x * x;
}

int gx(int x)
{
	std::this_thread::sleep_for(std::chrono::seconds(7));
	return x*x/2;
}

int main(int argc, char** argv)
{

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numOfProc);

	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (id == 0)
	{
		int x;
		std::cout << "Enter X value:\n";
		std::cin >> x;
		for(int p = 1; p<3; ++p)
			MPI_Send(&x, 1, MPI_INT, p, tag, MPI_COMM_WORLD);
		fProcess = clock();
		sProcess = clock();
	}

	if (id == 1)
	{
		int rec;
		MPI_Recv(&rec, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		int send = fx(rec);
		if(rec1)
			MPI_Send(&send, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}

	if (id == 2)
	{
		int rec;
		MPI_Recv(&rec, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		int send = gx(rec);
		if(rec2)
			MPI_Send(&send, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}

	if (id == 0)
	{
		int f = 0, g = 0;
		int flag1 = 0, flag2 = 0;
		while (rec1 || rec2)
		{
			if ((clock() - fProcess) / CLOCKS_PER_SEC > 5 && rec1)
			{
				std::cout << "Wanna stop first process?\n";
				int ans;
				std::cin >> ans;
				if (ans == 1)
				{ 
					f = 1;
					rec1 = false;
				}
				else
				{
					fProcess = clock();
				}
			}
			if ((clock() - sProcess) / CLOCKS_PER_SEC > 5 && rec2)
			{
				std::cout << "Wanna stop second process?\n";
				int ans;
				std::cin >> ans;
				if (ans == 1)
				{
					g = 1;
					rec2 = false;
				}
				else
				{
					sProcess = clock();
				}
			}
			if (rec1)
			{
				MPI_Iprobe(1, tag, MPI_COMM_WORLD, &flag1, &status);
				if (flag1)
				{
					MPI_Recv(&f, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
					if (f == 0)
						rec2 = false;
					rec1 = false;
				}
			}
			if (rec2)
			{
				MPI_Iprobe(2, tag, MPI_COMM_WORLD, &flag2, &status);
				if (flag2)
				{
					MPI_Recv(&g, 1, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);
					if (g == 0)
						rec1 = false;
					rec2 = false;
				}
			}
		}
		std::cout << "Result: " << f*g << std::endl;
	}
		MPI_Finalize();
		return 0;
}