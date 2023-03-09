#include "phonebook.h"

bool operator==(const phonebook &first, const phonebook &second)
{
	return strcmp(first.surname.c_str(), second.surname.c_str()) == 0 && strcmp(first.name.c_str(), second.name.c_str()) == 0 &&
		   strcmp(first.patronymic.c_str(), second.patronymic.c_str()) == 0 && first.number == second.number;
}

bool operator>(const phonebook &first, const phonebook &second)
{
	auto a = strcmp(first.surname.c_str(), second.surname.c_str());
	if (a > 0)
	{
		return true;
	}
	else if (a < 0)
	{
		return false;
	}
	auto b = strcmp(first.name.c_str(), second.name.c_str());
	if (b > 0)
	{
		return true;
	}
	else if (b < 0)
	{
		return false;
	}
	auto c = strcmp(first.patronymic.c_str(), second.patronymic.c_str());
	if (c > 0)
	{
		return true;
	}
	else if (c < 0)
	{
		return false;
	}
	return first.number > second.number;
}

bool operator>=(const phonebook &first, const phonebook &second)
{
	return first > second || first == second;
}

bool operator<(const phonebook &first, const phonebook &second)
{
	return !(first >= second);
}
