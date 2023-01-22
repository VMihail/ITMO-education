#pragma once

#include "LN.h"
#include "return_codes.h"

#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

inline LN readNext(stack< LN > &st)
{
	LN result = st.top();
	st.pop();
	return result;
}

void printStack(stack< LN > &st, ofstream &out)
{
	while (not st.empty())
	{
		out << st.top() << '\n';
		st.pop();
	}
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cerr << "Wrong args, specify only the name of the input and output file\n";
		return ERROR_INVALID_PARAMETER;
	}
	ifstream in;
	in.open(argv[1]);
	if (not in.is_open())
	{
		cerr << "Failed to open input file\n";
		return ERROR_FILE_NOT_FOUND;
		;
	}
	ofstream out;
	out.open(argv[2]);
	if (not out.is_open())
	{
		cerr << "Failed to open output file\n";
		return ERROR_FILE_NOT_FOUND;
	}
	string next;
	stack< LN > st;
	try
	{
		while (in >> next)
		{
			if (next == "+")
			{
				LN first = readNext(st);
				LN second = readNext(st);
				st.emplace(second + first);
			}
			else if (next == "-")
			{
				LN second = readNext(st);
				LN first = readNext(st);
				st.emplace(first - second);
			}
			else if (next == "*")
			{
				LN first = readNext(st);
				LN second = readNext(st);
				st.emplace(second * first);
			}
			else if (next == "/")
			{
				LN first = readNext(st);
				LN second = readNext(st);
				st.emplace(first / second);
			}
			else if (next == "%")
			{
				LN first = readNext(st);
				LN second = readNext(st);
				st.emplace(first % second);
			}
			else if (next == "~")
			{
				// no implementation
				LN first = readNext(st);
				st.emplace(-first);
			}
			else if (next == "_")
			{
				LN first = readNext(st);
				st.emplace(-first);
			}
			else if (next == "<")
			{
				LN second = readNext(st);
				LN first = readNext(st);
				st.emplace(first < second);
			}
			else if (next == "<=")
			{
				LN second = readNext(st);
				LN first = readNext(st);
				st.emplace(first <= second);
			}
			else if (next == ">")
			{
				LN second = readNext(st);
				LN first = readNext(st);
				st.emplace(first > second);
			}
			else if (next == ">=")
			{
				LN second = readNext(st);
				LN first = readNext(st);
				st.emplace(first >= second);
			}
			else if (next == "==")
			{
				LN second = readNext(st);
				LN first = readNext(st);
				st.emplace(first == second);
			}
			else if (next == "!=")
			{
				LN second = readNext(st);
				LN first = readNext(st);
				st.emplace(first != second);
			}
			else
			{
				st.emplace(next);
			}
		}
	} catch (bad_alloc &e)
	{
		cerr << "Failed to allocate memory, result:\n";
		in.close();
		printStack(st, out);
		out.close();
		return ERROR_MEMORY;
	} catch (...)
	{
		cerr << "Unknown error\n";
		in.close();
		printStack(st, out);
		out.close();
		return 2;
	}
	in.close();
	printStack(st, out);
	out.close();
	return 0;
}
