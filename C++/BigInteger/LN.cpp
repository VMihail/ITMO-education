#include "LN.h"

LN::LN(long long number)
{
	isNaN = false;
	isNegative = number < 0;
	if (number == 0)
	{
		digits.push_back(0);
	}
	number = std::abs(number);
	while (number > 0)
	{
		digits.push_back((digit)(number % 10));
		number /= 10;
	}
}

LN::LN(const char *s)
{
	isNaN = false;
	size_t len = strlen(s);
	if (len <= 0 or len == 3 and s[0] == 'N' and s[1] == 'a' and s[2] == 'N' /*s.substr(0, 3) == "NaN"*/)
	{
		isNaN = true;
		return;
	}
	ptrdiff_t end = 0;
	if (s[0] == '-' and s[1] != '0')
	{
		isNegative = true;
		++end;
	}
	else if (s[0] == '-' and s[1] == '0')
	{
		digits.push_back(0);
		return;
	}
	for (auto i = (ptrdiff_t)(len - 1); i >= end; i--)
	{
		digits.push_back((digit)(s[i] - 48));
	}
}

LN::LN(const string_view &s)
{
	isNaN = false;
	size_t len = s.length();
	if (len <= 0 or len == 3 and s.substr(0, 3) == "NaN")
	{
		isNaN = true;
		return;
	}
	ptrdiff_t end = 0;
	if (s[0] == '-' and s[1] != '0')
	{
		isNegative = true;
		++end;
	}
	else if (s[0] == '-' and s[1] == '0')
	{
		digits.push_back(0);
		return;
	}
	for (auto i = (ptrdiff_t)(len - 1); i >= end; i--)
	{
		digits.push_back((digit)(s[i] - 48));
	}
}

LN::LN(const LN &second)
{
	digits.resize(second.digits.size());
	for (size_t i = 0; i < second.digits.size(); i++)
	{
		digits[i] = second.digits[i];
	}
	isNaN = second.isNaN;
	isNegative = second.isNegative;
}

LN::LN(LN &&second) noexcept
{
	digits.resize(second.digits.size());
	for (size_t i = 0; i < second.digits.size(); i++)
	{
		digits[i] = second.digits[i];
	}
	isNaN = second.isNaN;
	isNegative = second.isNegative;
	second.digits.clear();
	second.isNaN = false;
	second.isNegative = false;
}

LN &LN::operator=(const LN &second)
{
	if (this == &second)
	{
		return *this;
	}
	digits.resize(second.digits.size());
	for (size_t i = 0; i < second.digits.size(); i++)
	{
		digits[i] = second.digits[i];
	}
	isNaN = second.isNaN;
	isNegative = second.isNegative;
	return *this;
}

LN &LN::operator=(LN &&second) noexcept
{
	digits.clear();
	digits.resize(second.digits.size());
	for (size_t i = 0; i < digits.size(); i++)
	{
		digits[i] = second.digits[i];
	}
	isNegative = second.isNegative;
	isNaN = second.isNaN;
	second.digits.clear();
	return *this;
}

int LN::compare(const LN &second) const
{
	int signFirst;
	if (digits.size() == 1 and digits[0] == 0 and not isNegative)
	{
		signFirst = 0;
	}
	else if (isNegative)
	{
		signFirst = -1;
	}
	else
	{
		signFirst = 1;
	}
	int signSecond;
	if (digits.size() == 1 and digits[0] == 0 and not isNegative)
	{
		signSecond = 0;
	}
	else if (isNegative)
	{
		signSecond = -1;
	}
	else
	{
		signSecond = 1;
	}
	if (signFirst != signSecond)
	{
		return signFirst - signSecond;
	}
	if (signFirst == 0 and signSecond == 0)
	{
		return 0;
	}
	if (digits.size() != second.digits.size())
	{
		if (signFirst == 1)
		{
			return digits.size() > second.digits.size() ? 1 : -1;
		}
		return second.digits.size() > digits.size() ? 1 : -1;
	}
	for (size_t i = digits.size() - 1; i >= 0; i--)
	{
		if (digits[i] == second.digits[i])
		{
			continue;
		}
		if (signFirst == 1)
		{
			return digits[i] > second.digits[i] ? 1 : -1;
		}
		return second.digits[i] > digits[i] ? 1 : -1;
	}
	return 0;
}

bool LN::isNaNBin(const LN &first, const LN &second)
{
	return first.isNaN or second.isNaN;
}

bool operator<(const LN &left, const LN &second)
{
	if (LN::isNaNBin(left, second))
	{
		return false;
	}
	return left.compare(second) < 0;
}

bool operator<=(const LN &first, const LN &second)
{
	if (LN::isNaNBin(first, second))
	{
		return false;
	}
	return first.compare(second) <= 0;
}

bool operator>(const LN &first, const LN &second)
{
	if (LN::isNaNBin(first, second))
	{
		return false;
	}
	return first.compare(second) > 0;
}

bool operator>=(const LN &first, const LN &second)
{
	if (LN::isNaNBin(first, second))
	{
		return false;
	}
	return first.compare(second) >= 0;
}

bool operator==(const LN &first, const LN &second)
{
	if (LN::isNaNBin(first, second))
	{
		return false;
	}
	return first.compare(second) == 0;
}

bool operator!=(const LN &first, const LN &second)
{
	if (LN::isNaNBin(first, second))
	{
		return true;
	}
	return first.compare(second) != 0;
}

LN::operator bool() const
{
	return isNaN or (not(digits.size() == 1 and digits[0] == 0));
}

LN::operator long long() const
{
	if (isNaN)
	{
		throw std::bad_cast();
	}
	long long inf = isNegative ? INT64_MIN : INT64_MAX;
	LN maxValue = LN(inf);
	if (not isNegative)
	{
		if (*this > maxValue)
		{
			throw std::bad_cast();
		}
	}
	else
	{
		if (*this < maxValue)
		{
			throw std::bad_cast();
		}
	}
	long long result = 0LL;
	long long pos = 1LL;
	for (size_t i = 0; i < digits.size(); i++)
	{
		result += (long long)digits[i] * pos;
		pos *= 10LL;
	}
	return isNegative ? -result : result;
}

LN operator""_ln(const char *s)
{
	return LN(s);
}

void LN::skip()
{
	for (size_t i = digits.size() - 1; i >= 0; i--)
	{
		if (digits[i] == 0)
		{
			continue;
		}
		digits.resize(i + 1);
		return;
	}
	digits.resize(1);
}

void LN::addNumbers(const LN &second)
{
	size_t newSize = digits.size() > second.digits.size() ? digits.size() : second.digits.size();
	digits.resize(newSize + 1);
	for (size_t i = 0; i < newSize; i++)
	{
		if (i < second.digits.size())
		{
			digits[i] += second.digits[i];
		}
		if (digits[i] > 9)
		{
			++digits[i + 1];
		}
		digits[i] %= 10;
	}
	digits[digits.size() - 1] %= 10;
}

int LN::compareAbs(const LN &that) const
{
	if (digits.size() > that.digits.size())
	{
		return 1;
	}
	if (digits.size() < that.digits.size())
	{
		return -1;
	}
	for (size_t i = digits.size(); i >= 0; i--)
	{
		if (digits[i] == that.digits[i])
		{
			continue;
		}
		return digits[i] > that.digits[i] ? 1 : -1;
	}
	return 0;
}

void LN::subNumbers(const LN &that)
{
	int more = compareAbs(that);
	if (more == 0)
	{
		digits.resize(1);
		digits[0] = 0;
		isNegative = false;
		isNaN = false;
		return;
	}
	if (more > 0)
	{
		for (size_t i = 0; i < digits.size(); i++)
		{
			if (i < that.digits.size())
			{
				digits[i] -= that.digits[i];
			}
			if (digits[i] < 0)
			{
				for (size_t j = i + 1; j < digits.size(); j++)
				{
					if (digits[j] != 0)
					{
						--digits[j];
						for (size_t e = i + 1; e < j; e++)
						{
							digits[e] = 9;
						}
						break;
					}
				}
				digits[i] += 10;
			}
		}
		return;
	}
	isNegative = not isNegative;
	vector< digit > result(that.digits.size());
	for (size_t i = 0; i < result.size(); i++)
	{
		result[i] = that.digits[i];
	}
	for (size_t i = 0; i < that.digits.size(); i++)
	{
		if (i < digits.size())
		{
			result[i] -= digits[i];
		}
		if (result[i] < 0)
		{
			for (size_t j = i + 1; j < that.digits.size(); j++)
			{
				if (result[j])
				{
					--result[j];
					for (size_t e = i + 1; e < j; e++)
					{
						result[e] = 9;
					}
					break;
				}
			}
			result[i] += 10;
		}
	}
	digits.clear();
	digits = result;
}

LN &LN::operator+=(const LN &second)
{
	if (LN::isNaNBin(*this, second))
	{
		isNaN = true;
		return *this;
	}
	if (isNegative == second.isNegative)
	{
		addNumbers(second);
	}
	else
	{
		subNumbers(second);
	}
	skip();
	return *this;
}

LN &LN::operator-=(const LN &second)
{
	if (LN::isNaNBin(*this, second))
	{
		isNaN = true;
		return *this;
	}
	if (isNegative == second.isNegative)
	{
		subNumbers(second);
	}
	else
	{
		addNumbers(second);
	}
	skip();
	return *this;
}

LN operator+(const LN &first, const LN &second)
{
	LN result(first);
	return result += second;
}

LN operator-(const LN &first, const LN &second)
{
	LN result(first);
	return result -= second;
}

LN &LN::operator-()
{
	isNegative = not isNegative;
	return *this;
}

LN operator-(const LN &second)
{
	LN result(second);
	result.isNegative = not result.isNegative;
	return result;
}

string LN::toString()
{
	if (isNaN)
	{
		return "NaN";
	}
	string result;
	if (isNegative)
	{
		result += "-";
	}
	for (ptrdiff_t i = (ptrdiff_t)digits.size() - 1; i >= 0; i--)
	{
		result += std::to_string((int16_t)digits[i]);
	}
	return result;
}

std::ostream &operator<<(std::ostream &out, const LN &number)
{
	if (number.isNaN)
	{
		out << "NaN";
		return out;
	}
	if (number.digits.empty() or (number.digits.size() == 1 and number.digits[0] == 0))
	{
		out << 0;
		return out;
	}
	if (number.isNegative)
	{
		out << "-";
	}
	for (size_t i = 0; i < number.digits.size(); i++)
	{
		out << static_cast< int >(number.digits[number.digits.size() - i - 1]);
	}
	return out;
}

std::istream &operator>>(std::istream &in, LN &number)
{
	std::string next;
	if (not(in >> next))
	{
		return in;
	}
	number = LN(next);
	return in;
}

LN &LN::operator++()
{
	return *this += 1_ln;
}

LN LN::operator++(int)
{
	LN save(*this);
	*this += 1_ln;
	return save;
}

LN &LN::operator--()
{
	return *this -= 1_ln;
}

LN LN::operator--(int)
{
	LN save(*this);
	*this -= 1_ln;
	return save;
}

// Atemp 2

LN operator*(const LN &first, const LN &second)
{
	if (LN::isNaNBin(first, second))
	{
		return LN("NaN");
	}
	if (first.digits.empty() or second.digits.empty() or (first.digits.size() == 1 and first.digits[0] == 0) or
		(second.digits.size() == 1 and second.digits[0] == 0))
	{
		return 0_ln;
	}
	LN result(0LL);
	result.isNegative = first.isNegative ^ second.isNegative;
	size_t newSize = first.digits.size() + second.digits.size();
	long long *newDigits = (long long *)malloc((newSize + 1) * sizeof(long long));
	if (!newDigits)
	{
		throw std::bad_alloc();
	}
	for (size_t i = 0; i < newSize + 1; i++)
	{
		newDigits[i] = 0;
	}
	for (size_t i = 0; i < first.digits.size(); i++)
	{
		for (size_t j = 0; j < second.digits.size(); j++)
		{
			newDigits[i + j] += first.digits[i] * second.digits[j];
		}
	}
	for (size_t i = 0; i < newSize; i++)
	{
		newDigits[i + 1] += newDigits[i] / 10;
		newDigits[i] %= 10;
	}
	result.digits.resize(newSize + 1, 0);
	for (size_t i = 0; i < newSize + 1; i++)
	{
		result.digits[i] = (LN::digit)newDigits[i];
	}
	free(newDigits);
	bool ok = false;
	for (size_t i = result.digits.size() - 1; i >= 0; i--)
	{
		if (result.digits[i] != 0)
		{
			result.digits.resize(i + 1);
			ok = true;
			break;
		}
	}
	if (not ok)
	{
		result.digits.resize(1);
	}
	return result;
}

LN &LN::operator*=(const LN &second)
{
	*this = (*this) * second;
	return *this;
}

LN operator/(const LN &first, const LN &second)
{
	if (LN::isNaNBin(first, second) or second.digits.empty() or second.digits.size() == 1 and second.digits[0] == 0)
	{
		return LN("NaN");
	}
	if (second.digits.size() == 1 and second.digits[0] == 1)
	{
		return second.isNegative ? -first : first;
	}
	if (first.compareAbs(second) < 0)
	{
		return 0_ln;
	}
	if (first.compareAbs(second) == 0)
	{
		return first.isNegative ^ second.isNegative ? -1_ln : 1_ln;
	}
	LN result;
	result.isNegative = first.isNegative ^ second.isNegative;
	result.digits.resize(first.digits.size());
	LN tmp = 0_ln;
	for (size_t i = 0; i < first.digits.size(); i++)
	{
		size_t index = first.digits.size() - i - 1;
		tmp.digits.insert(tmp.digits.begin() + 0, 0);
		tmp += LN((long long)first.digits[index]);
		result.digits[index] = 0;
		while (tmp.compareAbs(second) >= 0)
		{
			tmp += second.isNegative ? second : -second;
			result.digits[index]++;
		}
	}
	bool ok = false;
	for (size_t i = result.digits.size() - 1; i >= 0; i--)
	{
		if (result.digits[i] != 0)
		{
			result.digits.resize(i + 1);
			ok = true;
			break;
		}
	}
	if (not ok)
	{
		result.digits.resize(1);
	}
	return result;
}

LN operator%(const LN &first, const LN &second)
{
	if (LN::isNaNBin(first, second) or second.digits.empty() or second.digits.size() == 1 and second.digits[0] == 0)
	{
		return LN("NaN");
	}
	return first - first / second * second;
}

LN &LN::operator%=(const LN &second)
{
	*this = (*this) % second;
	return *this;
}

LN &LN::operator/=(const LN &second)
{
	*this = (*this) / second;
	return *this;
}

LN operator~(const LN &number)
{
	if (number.isNaN or number.isNegative)
	{
		return LN("NaN");
	}
	LN left = 0_ln;
	LN right = number;
	while (right > left + 1_ln)
	{
		auto middle = (left - right) / 2_ln + left;
		if (middle * middle <= number)
		{
			left = middle;
		}
		else
		{
			right = middle;
		}
	}
	return left;
}
