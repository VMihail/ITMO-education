#include "return_codes.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int gauss(size_t n, size_t m, float (*a)[m], float *resultVec);
void arrayFill(size_t n, ptrdiff_t *array, int value);
void printMatrix(size_t n, size_t m, const float (*matrix)[m]);

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Wrong args, specify only the name of the input and output file\n");
		return ERROR_INVALID_PARAMETER;
	}
	FILE *inputFile = fopen(argv[1], "r");
	if (!inputFile)
	{
		printf("Input file not found\n");
		return ERROR_FILE_NOT_FOUND;
	}
	size_t n;
	fscanf(inputFile, "%zd", &n);
	size_t m = n + 1;
	float(*matrix)[m] = malloc(n * m * sizeof(float));
	float *resultVec = malloc(n * sizeof(float));
	if (!matrix || !resultVec)
	{
		printf("Failed to allocate memory\n");
		fclose(inputFile);
		free(matrix);
		free(resultVec);
		return ERROR_MEMORY;
	}
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			fscanf(inputFile, "%f", &matrix[i][j]);
		}
	}
	fclose(inputFile);
	int result = gauss(n, m, matrix, resultVec);
	FILE *outputFile = fopen(argv[2], "w");
	if (!outputFile)
	{
		printf("Output file not found\n");
		free(matrix);
		free(resultVec);
		return ERROR_FILE_NOT_FOUND;
	}
	if (result == 0)
	{
		fprintf(outputFile, "no solution\n");
	}
	else if (result == INT32_MAX)
	{
		fprintf(outputFile, "many solutions\n");
	}
	else
	{
		for (size_t i = 0; i < n; i++)
		{
			fprintf(outputFile, "%f\n", resultVec[i]);
		}
	}
	free(matrix);
	free(resultVec);
	fclose(outputFile);
	return 0;
}

const float EPS = 1e-5f;

void arrayFill(size_t n, ptrdiff_t *array, int value)
{
	for (size_t i = 0; i < n; i++)
	{
		array[i] = value;
	}
}

void printMatrix(size_t n, size_t m, const float (*matrix)[m])
{
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			printf("%f ", matrix[i][j]);
		}
		printf("\n");
	}
}

int gauss(size_t n, size_t m, float (*a)[m], float *resultVec)
{
	--m;
	ptrdiff_t *p = malloc(m * sizeof(ptrdiff_t));
	arrayFill(n, p, -1);
	size_t row = 0;
	for (size_t col = 0; col < m; col++)
	{
		if (row >= n)
		{
			break;
		}
		size_t pv = row;
		for (size_t i = row; i < n; i++)
		{
			if (fabsf(a[i][col]) > fabsf(a[pv][col]))
			{
				pv = i;
			}
		}
		if (fabsf(a[pv][col]) < EPS)
		{
			continue;
		}
		for (size_t i = col; i < m + 1; i++)
		{
			float save = a[pv][i];
			a[pv][i] = a[row][i];
			a[row][i] = save;
		}
		p[col] = (ptrdiff_t)row;
		for (size_t i = 0; i < n; i++)
		{
			if (i == row)
			{
				continue;
			}
			float scalar = a[i][col] / a[row][col];
			for (size_t j = col; j < m + 1; j++)
			{
				a[i][j] -= a[row][j] * scalar;
			}
		}
		++row;
	}
	for (size_t i = 0; i < m; i++)
	{
		if (p[i] == -1)
		{
			continue;
		}
		resultVec[i] = a[p[i]][m] / a[p[i]][i];
	}
	for (size_t i = 0; i < n; i++)
	{
		float sum = 0.0f;
		for (size_t j = 0; j < m; j++)
		{
			sum += a[i][j] * resultVec[j];
		}
		if (fabsf(sum - a[i][m]) > EPS)
		{
			free(p);
			return 0;
		}
	}
	for (size_t i = 0; i < m; i++)
	{
		if (p[i] == -1)
		{
			free(p);
			return INT32_MAX;
		}
	}
	free(p);
	return 1;
}
