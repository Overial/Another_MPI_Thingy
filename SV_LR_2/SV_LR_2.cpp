// IDM-23-04
// Afanasyev Vadim
// LR№2

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include "mpi.h"

using namespace std;
using namespace std::chrono;

#define ROW_COUNT_A 8
#define COL_COUNT_A 5
#define COL_COUNT_B 3

#define BLOCK_ROWS 2

static void FillMatrices(int Matrix1[ROW_COUNT_A][COL_COUNT_A], int Matrix2[COL_COUNT_A][COL_COUNT_B])
{
	for (int i = 0; i < ROW_COUNT_A; ++i)
	{
		for (int j = 0; j < COL_COUNT_A; ++j)
		{
			Matrix1[i][j] = rand() % 10;
		}
	}

	for (int i = 0; i < COL_COUNT_A; ++i)
	{
		for (int j = 0; j < COL_COUNT_B; ++j)
		{
			Matrix2[i][j] = rand() % 10;
		}
	}
}

static void PrintMatrices(int Matrix1[ROW_COUNT_A][COL_COUNT_A], int Matrix2[COL_COUNT_A][COL_COUNT_B], int Result[ROW_COUNT_A][COL_COUNT_B])
{
	cout << "Matrix 1:" << endl;
	for (int i = 0; i < ROW_COUNT_A; ++i)
	{
		for (int j = 0; j < COL_COUNT_A; ++j)
		{
			cout << setw(4) << Matrix1[i][j] << "|";
		}
		cout << endl;
	}

	cout << "Matrix 2:" << endl;
	for (int i = 0; i < COL_COUNT_A; ++i)
	{
		for (int j = 0; j < COL_COUNT_B; ++j)
		{
			cout << setw(4) << Matrix2[i][j] << "|";
		}
		cout << endl;
	}

	cout << "Multiplication result:" << endl;
	for (int i = 0; i < ROW_COUNT_A; ++i)
	{
		for (int j = 0; j < COL_COUNT_B; ++j)
		{
			cout << setw(4) << Result[i][j] << "|";
		}
		cout << endl;
	}
}

void SingleProcessMult(int Matrix1[ROW_COUNT_A][COL_COUNT_A], int Matrix2[COL_COUNT_A][COL_COUNT_B], int Result[ROW_COUNT_A][COL_COUNT_B])
{
	// Столбцы второй матрицы
	for (int k = 0; k < COL_COUNT_B; ++k)
	{
		// Строки первой матрицы
		for (int i = 0; i < ROW_COUNT_A; ++i)
		{
			// Столбцы первой матрицы
			for (int j = 0; j < COL_COUNT_A; ++j)
			{
				Result[i][k] += Matrix1[i][j] * Matrix2[j][k];
			}
		}
	}
}

int main()
{
	int Rank = 0;
	int ProcessCount = 0;

	MPI_Status Status;

	// Запускаем MPI
	MPI_Init(NULL, NULL);

	// Читаем инфу о процессах
	MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcessCount);

	int WorkerProcessCount = ProcessCount - 1;

	int MatrixA[ROW_COUNT_A][COL_COUNT_A] = { 0 };
	int MatrixB[COL_COUNT_A][COL_COUNT_B] = { 0 };
	int Result[ROW_COUNT_A][COL_COUNT_B] = { 0 };

	srand(static_cast<unsigned int>(time(NULL)));
	steady_clock::time_point StartTime = high_resolution_clock::now();

	// Главный процесс
	if (Rank == 0)
	{
		// FillMatrices();

		// Первая матрица для отладки

		MatrixA[0][0] = 1;
		MatrixA[0][1] = 2;
		MatrixA[0][2] = 3;
		MatrixA[0][3] = 4;
		MatrixA[0][4] = 5;

		MatrixA[1][0] = 6;
		MatrixA[1][1] = 7;
		MatrixA[1][2] = 8;
		MatrixA[1][3] = 9;
		MatrixA[1][4] = 1;

		MatrixA[2][0] = 2;
		MatrixA[2][1] = 3;
		MatrixA[2][2] = 4;
		MatrixA[2][3] = 5;
		MatrixA[2][4] = 6;

		MatrixA[3][0] = 7;
		MatrixA[3][1] = 8;
		MatrixA[3][2] = 9;
		MatrixA[3][3] = 1;
		MatrixA[3][4] = 2;
		
		MatrixA[4][0] = 3;
		MatrixA[4][1] = 4;
		MatrixA[4][2] = 5;
		MatrixA[4][3] = 6;
		MatrixA[4][4] = 7;

		MatrixA[5][0] = 8;
		MatrixA[5][1] = 9;
		MatrixA[5][2] = 3;
		MatrixA[5][3] = 2;
		MatrixA[5][4] = 1;

		MatrixA[6][0] = 4;
		MatrixA[6][1] = 4;
		MatrixA[6][2] = 7;
		MatrixA[6][3] = 8;
		MatrixA[6][4] = 9;

		MatrixA[7][0] = 1;
		MatrixA[7][1] = 2;
		MatrixA[7][2] = 4;
		MatrixA[7][3] = 3;
		MatrixA[7][4] = 5;

		// Вторая матрица для отладки

		MatrixB[0][0] = 7;
		MatrixB[0][1] = 5;
		MatrixB[0][2] = 8;

		MatrixB[1][0] = 4;
		MatrixB[1][1] = 5;
		MatrixB[1][2] = 2;

		MatrixB[2][0] = 1;
		MatrixB[2][1] = 5;
		MatrixB[2][2] = 6;

		MatrixB[3][0] = 7;
		MatrixB[3][1] = 3;
		MatrixB[3][2] = 9;

		MatrixB[4][0] = 5;
		MatrixB[4][1] = 1;
		MatrixB[4][2] = 2;

		// Ожидаемый результат:
		// 71 | 47 | 76
		// 146| 133| 193
		// 95 | 66 | 103
		// 107| 125| 139
		// 119| 85 | 130
		// 114| 107| 120
		// 152| 108| 172
		// 65 | 49 | 73

		// При запуске на одном процессе
		if (ProcessCount == 1)
		{
			steady_clock::time_point SingleStartTime = high_resolution_clock::now();

			SingleProcessMult(MatrixA, MatrixB, Result);

			steady_clock::time_point SingleEndTime = high_resolution_clock::now();
			long long SingleTotalTime = duration_cast<microseconds>(SingleEndTime - SingleStartTime).count();

			cout << "SINGLE PROCESS MODE" << endl;
			PrintMatrices(MatrixA, MatrixB, Result);
			cout << "Total time: " << SingleTotalTime << endl;

			MPI_Finalize();

			return 0;
		}
	}

	// Раздаем пацанам вторую матрицу
	MPI_Bcast(MatrixB, COL_COUNT_A * COL_COUNT_B, MPI_INT, 0, MPI_COMM_WORLD);

	// Фрагмент первой матрицы
	int BlockMatrixA[BLOCK_ROWS][COL_COUNT_A] = { 0 };

	// Раздаем пацанам таски по первой матрице
	MPI_Scatter(MatrixA, BLOCK_ROWS* COL_COUNT_A, MPI_INT, BlockMatrixA, BLOCK_ROWS* COL_COUNT_A, MPI_INT, 0, MPI_COMM_WORLD);

	// Фрагмент результирующей матрицы
	int BlockResult[BLOCK_ROWS][COL_COUNT_B] = { 0 };

	// Пацаны пилят таски
	for (int i = 0; i < BLOCK_ROWS; ++i)
	{
		for (int j = 0; j < COL_COUNT_B; ++j)
		{
			for (int k = 0; k < COL_COUNT_A; ++k)
			{
				BlockResult[i][j] += BlockMatrixA[i][k] * MatrixB[k][j];
			}
		}
	}

	// You shall not pass
	MPI_Barrier(MPI_COMM_WORLD);

	// Пацаны отчитываются о проделанной работе
	MPI_Gather(BlockResult, BLOCK_ROWS* COL_COUNT_B, MPI_INT, Result, BLOCK_ROWS* COL_COUNT_B, MPI_INT, 0, MPI_COMM_WORLD);

	steady_clock::time_point EndTime = high_resolution_clock::now();
	long long TotalTime = duration_cast<microseconds>(EndTime - StartTime).count();

	if (Rank == 0)
	{
		cout << "MPI MODE USING " << WorkerProcessCount << " WORKER PROCESSES" << endl;
		
		PrintMatrices(MatrixA, MatrixB, Result);
		
		cout << "Total time: " << TotalTime << endl;
	}

	// По домам
	MPI_Finalize();

	return 0;
}
