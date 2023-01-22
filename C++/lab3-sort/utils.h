#include <fstream>
#include <stdlib.h>

template< typename T >
void printArray(T *array, size_t len, std::ofstream &out)
{
	for (size_t i = 0; i < len; i++)
	{
		out << array[i] << '\n';
	}
}

template< typename T >
T *nextArray(size_t len, std::ifstream &in)
{
	T *result = (T *)malloc(len * sizeof(T));
	if (!result)
	{
		return nullptr;
	}
	for (size_t i = 0; i < len; i++)
	{
		in >> result[i];
	}
	return result;
}
