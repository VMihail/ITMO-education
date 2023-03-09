#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct phonebook
{
	string surname;
	string name;
	string patronymic;
	unsigned long long number;

	phonebook(string surname, string name, string patronymic, unsigned long long number)
	{
		this->surname = move(surname);
		this->name = move(name);
		this->patronymic = move(patronymic);
		this->number = number;
	}

	phonebook()
	{
		surname = nullptr;
		name = nullptr;
		patronymic = nullptr;
		number = 0LL;
	}

	void print(ofstream &out) const { out << surname << ' ' << name << ' ' << patronymic << ' ' << number << '\n'; }
};

bool operator==(const phonebook &first, const phonebook &second);

bool operator>(const phonebook &first, const phonebook &second);

bool operator>=(const phonebook &first, const phonebook &second);

bool operator<(const phonebook &first, const phonebook &second);
