#pragma once

#include "phonebook.h"
#include "quicksort.h"
#include "return_codes.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <utility>

using namespace std;

template< typename T >
int solve(size_t n, ifstream &in, char *outputFileName, bool descending)
{
	T *array = nextArray< T >(n, in);
	if (!array)
	{
		cerr << "Failed to allocate memory\n";
		return ERROR_MEMORY;
	}
	if (descending)
	{
		quickSort< T, true >(array, n);
	}
	else
	{
		quickSort< T, false >(array, n);
	}
	ofstream out(outputFileName);
	if (!out.is_open())
	{
		cerr << "Failed to open output file\n";
		return ERROR_FILE_NOT_FOUND;
	}
	printArray< T >(array, n, out);
	free(array);
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cerr << "Wrong args, specify only the name of the input and output file\n";
		return ERROR_INVALID_PARAMETER;
	}
	ifstream in(argv[1]);
	if (!in.is_open())
	{
		cerr << "Failed to open input file\n";
		return ERROR_FILE_NOT_FOUND;
	}
	string type, comp;
	in >> type >> comp;
	bool descending = (bool)(comp == "descending");
	size_t n;
	in >> n;
	int result = 0;
	if (type == "int")
	{
		result = solve< int >(n, in, argv[2], descending);
	}
	else if (type == "float")
	{
		result = solve< float >(n, in, argv[2], descending);
	}
	else if (type == "phonebook")
	{
		auto *array = (phonebook *)malloc(n * sizeof(phonebook));
		if (!array)
		{
			cerr << "Failed to allocate memory\n";
			return ERROR_MEMORY;
		}
		for (size_t i = 0; i < n; i++)
		{
			string surname, name, patronymic;
			unsigned long long number;
			in >> surname >> name >> patronymic >> number;
			array[i] = { surname, name, patronymic, number };
		}
		if (descending)
		{
			quickSort< phonebook, true >(array, n);
		}
		else
		{
			quickSort< phonebook, false >(array, n);
		}
		ofstream out(argv[2]);
		if (!out.is_open())
		{
			cerr << "Failed to open output file\n";
			return ERROR_FILE_NOT_FOUND;
		}
		for (size_t i = 0; i < n; i++)
		{
			array[i].print(out);
		}
		free(array);
		out.close();
	}
	in.close();
	return result == 0 ? 0 : ERROR_MEMORY;
}

/*
  phonebook
  ascending
  5
  aaa aaaa aaaaa 10010
  aa bb cc 255
  Aa bB cc 265
  z z z 1
  g g g 10
 */
