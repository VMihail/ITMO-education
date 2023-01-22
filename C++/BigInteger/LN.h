#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class LN
{
  private:
	typedef int8_t digit;
	vector< digit > digits;
	bool isNegative, isNaN;

	void skip();
	void addNumbers(const LN &second);
	void subNumbers(const LN &second);
	[[nodiscard]] int compareAbs(const LN &that) const;

  public:
	explicit LN(long long number = 0LL);
	explicit LN(const char *s);
	explicit LN(const string_view &s);

	~LN() { digits.clear(); }

	LN(const LN &second);
	LN(LN &&second) noexcept;

	LN &operator=(const LN &second);
	LN &operator=(LN &&second) noexcept;
	string toString();

	static bool isNaNBin(const LN &first, const LN &second);

	[[nodiscard]] int compare(const LN &second) const;

	friend bool operator<(const LN &first, const LN &second);
	friend bool operator<=(const LN &first, const LN &second);
	friend bool operator>(const LN &first, const LN &second);
	friend bool operator>=(const LN &first, const LN &second);
	friend bool operator==(const LN &first, const LN &second);
	friend bool operator!=(const LN &first, const LN &second);

	operator long long() const;
	operator bool() const;

	friend LN operator""_ln(const char *s);

	LN &operator+=(const LN &second);
	LN &operator-=(const LN &second);
	LN &operator*=(const LN &second);
	LN &operator/=(const LN &second);
	LN &operator%=(const LN &second);

	friend LN operator+(const LN &first, const LN &second);
	friend LN operator-(const LN &first, const LN &second);
	friend LN operator*(const LN &first, const LN &second);
	friend LN operator/(const LN &first, const LN &second);
	friend LN operator%(const LN &first, const LN &second);
	LN &operator-();
	friend LN operator-(const LN &second);

	friend std::ostream &operator<<(std::ostream &out, const LN &number);
	friend std::istream &operator>>(std::istream &in, LN &number);

	LN &operator++();
	LN operator++(int);

	LN &operator--();
	LN operator--(int);

	void setNegative(bool value) { isNegative = value; }

	bool getNegative() { return isNegative; }

	friend LN operator~(const LN &x);
};
