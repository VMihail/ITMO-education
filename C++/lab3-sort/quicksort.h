#include <stdlib.h>
#include <time.h>

using namespace std;

template< typename T >
void reverse(T *array, size_t len)
{
	for (size_t i = 0; i < (len >> 1); i++)
	{
		swap(array[i], array[len - 1 - i]);
	}
}

void activation()
{
	static bool start = false;
	if (!start)
	{
		srand(time(nullptr));
		start = true;
	}
}

size_t randomInRange(size_t left, size_t right)
{
	activation();
	return left + rand() % ((right + 1) - left);
}

template< typename T >
void quickSort(T *a, size_t left, size_t right)
{
	if (left >= right)
	{
		return;
	}
	T pivot = a[randomInRange(left, right)];
	size_t first = left, second = right;
	while (first <= second)
	{
		while (a[first++] < pivot)
			;
		while (a[second--] > pivot)
			;
		--first, ++second;
		if (first >= second)
		{
			break;
		}
		swap(a[first++], a[second--]);
	}
	size_t middle = second;
	quickSort(a, left, middle);
	quickSort(a, middle + 1, right);
}

template< typename T, bool descending >
void quickSort(T *a, size_t n)
{
	quickSort(a, 0, n - 1);
	if (descending)
	{
		reverse(a, n);
	}
}
