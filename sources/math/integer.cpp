/*
 *	Copyright (C) 2012-2013, The BCE Project authors. All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions
 *	are met:
 *	1. Redistributions of source code must retain the above copyright
 *	   notice, this list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright
 *	   notice, this list of conditions and the following disclaimer in the
 *	   documentation and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *	ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 *	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *	OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *	OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *	SUCH DAMAGE.
*/

#include <iostream>
#include <cmath>
#include <string>
#include <cstring>
#include <iterator>
#include <common/exception.hpp>
#include <math/integer.hpp>

using namespace std;

/*  Internal macro  */
/*  __get_bitlength(number): Get the length of a positive integer  */
#ifdef __get_bitlength
	#undef __get_bitlength
#endif
#define __get_bitlength(number) (log10(number) + 1)

/*  __max(a, b): Get the maximum number of two integers (@a, @b)  */
#ifdef __max
	#undef __max
#endif
#define __max(a,b) ((a) > (b) ? (a) : (b))

/*  __integer_is_zero(number): A faster way to check whether a @number equals to zero  */
/*  Internal use only!  */
#ifdef __integer_is_zero
	#undef __integer_is_zero
#endif
#define __integer_is_zero(number) ((number).length == 1 && (number).bits[0] == '\0')

/*  __integer_check_zero(number): Check whether a @number equals to zero, if so, set it's negative sign to false  */
#ifdef __integer_check_zero
	#undef __integer_check_zero
#endif
#define __integer_check_zero(number) {\
	if (__integer_is_zero(*number)) {\
		number->absolute();\
	}\
}

/*
 *	integer::integer()
 *
 *	Constructor.
 */
integer::integer() {
	/*  Set number to zero  */
	length = 1;

	bits = NULL;
	do {
		bits = new char[length]();
	} while(bits == NULL);

	negative = false;
}

/*
 *	integer::integer(const integer &src)
 *
 *	Copy constructor.
 */
integer::integer(const integer &src) {
	length = src.length;

	bits = NULL;
	do {
		bits = new char[length];
	} while(bits == NULL);

	memcpy(bits, src.bits, length * sizeof(char));
	negative = src.negative;

	__integer_check_zero(this);
}

/*
 *	integer::integer(const int src)
 *
 *	Construct from type 'int'.
 */
integer::integer(const int src) {
	bits = NULL;
	setValue(src);
}

/*
 *	integer::integer(const string &src)
 *
 *	Construct from type 'string'.
 */
integer::integer(const string &src) {
	bits = NULL;
	setValue(src);
	__integer_check_zero(this);
}

/*
 *	integer::~integer()
 *
 *	Destructor.
 */
integer::~integer() {
	if (bits != NULL) {
		delete []bits;
		bits = NULL;
	}
}

/*
 *	string integer::toString()
 *
 *	Convert the integer to type 'string'.
 */
string integer::toString() {
	string ret = "";
	char *p;

	if (negative == true) {
		ret += INTEGER_NEGATIVE_STRING;
	}

	for (p = bits + length - 1; p >= bits; p--) {
		ret.append(1, '0' + *p);
	}

	return(ret);
}

/*
 *	int integer::toInt()
 *
 *	Convert the integer to type 'string'.
 */
int integer::toInt() {
	int r = 0;
	char *p;

	for (p = bits + length - 1; p >= bits; p--) {
		r = r * 10 + static_cast<int>(*p);
	}

	return(r);
}

/*
 *	string integer::print()
 *
 *	Print the integer without endline character.
 */
void integer::print() {
	cout<<toString();
}

/*
 *	string integer::printLine()
 *
 *	Print the integer with endline character.
 */
void integer::printLine() {
	cout<<toString()<<endl;
}

/*
 *	void integer::setValue(const string &value)
 *
 *	Set the integer value from type 'string'.
 */
void integer::setValue(const string &value) {
	string::reverse_iterator pread, pend;
	char *pwrite;

	if (value.length() == 0) {
		setValue(0);
	}

	if (bits != NULL) {
		delete []bits;
	}

	pend = const_cast<string&>(value).rend();
	if (value.at(0) == '-') {
		length = value.length() - 1;
		negative = true;
		pend--;
	} else {
		length = value.length();
		negative = false;
	}

	do {
		bits = new char[length];
	} while(bits == NULL);

	for (pread = const_cast<string&>(value).rbegin(), pwrite = bits; pread != pend; pread++, pwrite++) {
		*pwrite = (*pread) - '0';
	}

	/*  Remove the preamble zero  */
	while(bits[length - 1] == '\0' && length > 1) {
		length--;
	}
}

/*
 *	void integer::setValue(const int value)
 *
 *	Set the integer value from type 'int'.
 */
void integer::setValue(const int value) {
	char *pwrite;
	int tmp_value = value;

	if (bits != NULL) {
		delete []bits;
	}

	if (tmp_value < 0) {
		tmp_value = -tmp_value;
		negative = true;
		length = __get_bitlength(tmp_value);
	} else if (tmp_value == 0) {
		negative = false;
		length = 1;
	} else {
		negative = false;
		length = __get_bitlength(tmp_value);
	}

	do {
		bits = new char[length];
	} while(bits == NULL);

	for (pwrite = bits; pwrite < bits + length; pwrite++) {
		*pwrite = static_cast<char>(tmp_value % 10);
		tmp_value /= 10;
	}
}

/*
 *	void integer::setValue(const integer &src)
 *
 *	Set the integer value from another integer.
 */
void integer::setValue(const integer &src) {
	if (bits != NULL) {
		delete []bits;
	}

	length = src.length;
	bits = NULL;

	do {
		bits = new char[length];
	} while(bits == NULL);

	memcpy(bits, src.bits, length * sizeof(char));
	negative = src.negative;
}

/*
 *	void integer::plus(const integer &na, const integer &nb, integer *dest)
 *
 *	Do plus operation `[ptr:@dest] = @na + @nb`.
 */
void integer::plus(const integer &na, const integer &nb, integer *dest) {
	char *px, *pna, *pnb;
	integer a, b;

	/*  Destruct destination integer  */
	if (dest->bits != NULL) {
		delete [](dest->bits);
		dest->bits = NULL;
	}

	if (na.negative == true && nb.negative == true) {
		/*  -|na|+(-|nb|) = -(|na|+|nb|)  */
		a = na;
		b = nb;
		a.negative = false;
		b.negative = false;
		plus(a, b, dest);
		dest->negative = true;
		__integer_check_zero(dest);
		return;
	}

	if (na.negative == false && nb.negative == true) {
		/*  |na|+(-|nb|) = |na|-|nb|  */
		b = nb;
		b.negative = false;
		minus(na, b, dest);
		__integer_check_zero(dest);
		return;
	}

	if (na.negative == true && nb.negative == false) {
		/*  -|na|+|nb| = |nb|+(-|na|)  */
		plus(nb, na, dest);
		__integer_check_zero(dest);
		return;
	}

	/*  Pre-calculate answer length and negative sign  */
	dest->negative = false;
	dest->length = __max(na.length, nb.length) + 1;

	/*  Allocate memory  */
	do {
		dest->bits = new char[dest->length]();
	} while(dest->bits == NULL);

	/*  Calculate from low bit to high bit  */
	for (px = dest->bits + 1, pna = na.bits, pnb = nb.bits; px < dest->bits + dest->length; px++, pna++, pnb++) {
		/*  Do plus operation and carry  */
		*(px - 1) += ((pna < na.bits + na.length) ? *pna : 0) + ((pnb < nb.bits + nb.length) ? *pnb : 0);
		*px += *(px - 1) / 10;
		*(px - 1) %= 10;
	}

	/*  Remove the preamble zero  */
	while(dest->length > 1 && dest->bits[dest->length - 1] == '\0') {
		dest->length--;
	}

	__integer_check_zero(dest);
}

/*
 *	void integer::minus(const integer &na, const integer &nb, integer *dest)
 *
 *	Do minus operation `[ptr:@dest] = @na - @nb`.
 */
void integer::minus(const integer &na, const integer &nb, integer *dest) {
	char *px, *pna, *pnb;
	integer a, b;

	/*  Destruct destination integer  */
	if (dest->bits != NULL) {
		delete [](dest->bits);
		dest->bits = NULL;
	}

	if (na.negative == true && nb.negative == true) {
		/*  -|na|-(-|nb|)=|nb|-|na|  */
		a = nb;
		a.negative = false;
		plus(a, na, dest);
		__integer_check_zero(dest);
		return;
	}

	if (na.negative == false && nb.negative == true) {
		/*  |na|-(-|nb|)=|na|+|nb|  */
		b = nb;
		b.negative = false;
		plus(na, b, dest);
		__integer_check_zero(dest);
		return;
	}

	if (na.negative == true && nb.negative == false) {
		/*  na-nb=-(nb-na)  */
		minus(nb, na, dest);
		dest->negative = true;
		__integer_check_zero(dest);
		return;
	}

	/*  Pre-calculate answer length  */
	dest->length = __max(na.length, nb.length);
	switch (compare(na, nb)) {
	case -1:
		/*  na < nb (na, nb > 0)=> r = -(nb - na)  */
		minus(nb, na, dest);
		dest->negative = true;
		__integer_check_zero(dest);
		return;
	case 0:
		dest->setValue(0);
		return;
	case 1:
		dest->negative = false;
		break;
	default:
		throw xsvException(ERROR_UNREACHABLE, __FILE__, __LINE__, __FUNCTION__, "Unreachable condition.");
		break;
	}

	/*  Allocate memory  */
	do {
		dest->bits = new char[dest->length]();
	} while(dest->bits == NULL);

	/*  Calculate from low bit to high bit  */
	for (px = dest->bits + 1, pna = na.bits, pnb = nb.bits; px <= dest->bits + dest->length; px++, pna++, pnb++) {
		/*  Do minus operation and borrow  */
		*(px - 1) += ((pna < na.bits + na.length) ? *pna : 0) - ((pnb < nb.bits + nb.length) ? *pnb : 0);
		if (*(px - 1) < 0) {
			*(px - 1) += 10;
			*px = -1;
		}
	}

	/*  Remove the preamble zero  */
	while(dest->bits[dest->length - 1] == '\0' && dest->length > 1) {
		dest->length--;
	}

	__integer_check_zero(dest);
}

/*
 *	void integer::multiply(const integer &na, const integer &nb, integer *dest)
 *
 *	Do multiply operation `[ptr:@dest] = @na * @nb`.
 */
void integer::multiply(const integer &na, const integer &nb, integer *dest) {
	size_t ofx_a, ofx_b;

	/*  Destruct destination integer  */
	if (dest->bits != NULL) {
		delete [](dest->bits);
		dest->bits = NULL;
	}

	/*  Pre-calculate answer length and negative sign  */
	dest->negative = (na.negative == nb.negative) ? false : true;
	dest->length = na.length + nb.length;

	/*  Allocate memory  */
	do {
		dest->bits = new char[dest->length]();
	} while(dest->bits == NULL);

	/*  Calculate from low bit to high bit  */
	for (ofx_a = 0; ofx_a < na.length; ofx_a++) {
		for (ofx_b = 0; ofx_b < nb.length; ofx_b++) {
			dest->bits[ofx_a + ofx_b] += na.bits[ofx_a] * nb.bits[ofx_b];
			dest->bits[ofx_a + ofx_b + 1] += dest->bits[ofx_a + ofx_b] / 10;
			dest->bits[ofx_a + ofx_b] %= 10;
		}
	}

	/*  Remove the preamble zero  */
	while(dest->bits[dest->length - 1] == '\0' && dest->length > 1) {
		dest->length--;
	}
	__integer_check_zero(dest);
}

/*
 *	void integer::divide(const integer &na, const integer &nb, integer *quotient, integer *mod)
 *
 *	Do divide operation `[ptr:@quotient]...[ptr:@mod] = @na / @nb`.
 */
void integer::divide(const integer &na, const integer &nb, integer *quotient, integer *mod) {
	char left, right, mid;
	char *pguess;
	integer t, a, b, c;

	if (__integer_is_zero(nb)) {
		throw xsvException(ERROR_DIVIDE_ZERO, __FILE__, __LINE__, __FUNCTION__, "Divide by zero.");
	}

	/*  Destruct destination(@quotient) integer  */
	if (quotient->bits != NULL) {
		delete [](quotient->bits);
		quotient->bits = NULL;
	}

	/*  Destruct destination(@mod) integer  */
	if (mod != NULL) {
		if (mod->bits != NULL) {
			delete [](mod->bits);
			mod->bits = NULL;
		}
	}

	if (na.negative == true || nb.negative == true) {
		/*  P.S: Main process doesn't accept negative number, so 
		 *       we remove the negative sign, calculate it and add 
		 *       negative sign to the answer manually.
		 */
		a = na;
		b = nb;
		a.negative = false;
		b.negative = false;
		divide(a, b, quotient, mod);
		quotient->negative = (na.negative == nb.negative) ? false : ((quotient->length == 1 && quotient->bits[0] == '\0') ? false : true);
		__integer_check_zero(quotient);
		if (mod != NULL) {
			mod->negative = na.negative;
			__integer_check_zero(mod);
		}
		return;
	}

	if (compare(na, nb) == -1) {
		/*  When |na| < |nb|, |na| / |nb| must be zero  */
		quotient->setValue(0);
	} else {
		/*  Pre-calculate answer length and negative sign  */
		quotient->length = na.length - nb.length + 1;
		quotient->negative = false;

		/*  Allocate memory  */
		do {
			quotient->bits = new char[quotient->length]();
		} while(quotient->bits == NULL);

		for (pguess = quotient->bits + quotient->length - 1; pguess >= quotient->bits; pguess--) {
			/*  Reset the left/right separator  */
			left = static_cast<char>(0);
			right = static_cast<char>(9);

			/*  Calculate the value of each bit by using Dichotomy method  */
			do {
				if (left == right) {
					*pguess = left;
					break;
				}

				if (left + 1 == right) {
					/*  Test left side separator  */
					*pguess = left;
					multiply(*quotient, nb, &t);

					if (compare(t, na) == 0) {
						goto break_all;
					}

					/*  Test right side separator  */
					*pguess = right;
					multiply(*quotient, nb, &t);
					switch(compare(t, na)) {
					case -1:
						*pguess = right;
						break;
					case 0:
						*pguess = right;
						goto break_all;
					case 1:
						*pguess = left;
						break;
					default:
						throw xsvException(ERROR_UNREACHABLE, __FILE__, __LINE__, __FUNCTION__, "Unreachable condition.");
						break;
					}
					break;
				}

				/*  Set new separator  */
				mid = (left + right) >> 1;
				*pguess = mid;
				multiply(*quotient, nb, &t);
				switch (compare(t, na)) {
				case -1:
					left = mid;
					break;
				case 0:
					goto break_all;
					break;
				case 1:
					right = mid;
					break;
				default:
					throw xsvException(ERROR_UNREACHABLE, __FILE__, __LINE__, __FUNCTION__, "Unreachable condition.");
					break;
				}
			} while(true);
		}
	}
break_all:
	/*  Remove the preamble zero  */
	while(quotient->bits[quotient->length - 1] == '\0' && quotient->length > 1) {
		quotient->length--;
	}

	__integer_check_zero(quotient);

	/*  Calculate the mod  */
	if (mod != NULL) {
		multiply(*quotient, nb, &t);
		minus(na, t, mod);
		__integer_check_zero(mod);
	}
}

/*
 *	int integer::compare(const integer &na, const integer &nb)
 *
 *	Compare two integer @na and @nb.
 *
 *	Return values:
 *	    -1 => @na < @nb
 *	     0 => @na = @nb
 *	     1 => @na > @nb
 */
int integer::compare(const integer &na, const integer &nb) {
	char *pna, *pnb;
	integer a, b;

	if (na.negative == true && nb.negative == true) {
		a = na;
		b = nb;
		a.negative = false;
		b.negative = false;
		return(compare(b, a));
	}

	if (na.negative == false && nb.negative == true) {
		return(1);
	}

	if (na.negative == true && nb.negative == false) {
		return(-1);
	}

	if (na.length < nb.length) {
		return(-1);
	} else if (na.length > nb.length) {
		return(1);
	} else {
		/*  Compare from high bit to low bit  */
		for (pna = na.bits + na.length - 1, pnb = nb.bits + nb.length - 1; pna >= na.bits; pna--, pnb--) {
			if (*pna != *pnb) {
				return((*pna < *pnb) ? -1 : 1);
			}
		}
		return(0);
	}
}

/*
 *	void integer::absolute()
 *
 *	Get absolute value of the integer.
 */
void integer::absolute() {
	negative = false;
}

/*
 *	void integer::opposite()
 *
 *	Get opposite value of the integer.
 */
void integer::opposite() {
	negative = !negative;
}

/*
 *	bool integer::isZero()
 *
 *	Get whether it is zero.
 */
bool integer::isZero() {
	if (__integer_is_zero(*this)) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool integer::isNegative()
 *
 *	Get whether it is a negative number.
 */
bool integer::isNegative() {
	return(negative);
}

/*
 *	integer& integer::operator=(const integer &src)
 *
 *	Overload operator `[ptr: this] = @src`
 */
integer& integer::operator=(const integer &src) {
	if (this == &src) {
		return(*this);
	}

	setValue(src);

	return(*this);
}

/*
 *	bool integer::operator==(const integer &src)
 *
 *	Overload operator `[ptr: this] == @src`
 */
bool integer::operator==(const integer &src) {
	if (compare(*this, src) == 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool integer::operator!=(const integer &src)
 *
 *	Overload operator `[ptr: this] != @src`
 */
bool integer::operator!=(const integer &src) {
	if (compare(*this, src) != 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool integer::operator<(const integer &src)
 *
 *	Overload operator `[ptr: this] < @src`
 */
bool integer::operator<(const integer &src) {
	if (compare(*this, src) < 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool integer::operator<=(const integer &src)
 *
 *	Overload operator `[ptr: this] <= @src`
 */
bool integer::operator<=(const integer &src) {
	if (compare(*this, src) <= 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool integer::operator>(const integer &src)
 *
 *	Overload operator `[ptr: this] > @src`
 */
bool integer::operator>(const integer &src) {
	if (compare(*this, src) > 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool integer::operator>=(const integer &src)
 *
 *	Overload operator `[ptr: this] >= @src`
 */
bool integer::operator>=(const integer &src) {
	if (compare(*this, src) >= 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	integer& integer::operator+(const integer &rval)
 *
 *	Overload operator `r = [ptr: this] + @rval`
 */
integer integer::operator+(const integer &rval) {
	integer r;

	plus(*this, rval, &r);

	return(r);
}

/*
 *	integer& integer::operator-(const integer &rval)
 *
 *	Overload operator `r = [ptr: this] - @rval`
 */
integer integer::operator-(const integer &rval) {
	integer r;

	minus(*this, rval, &r);

	return(r);
}

/*
 *	integer& integer::operator*(const integer &rval)
 *
 *	Overload operator `r = [ptr: this] * @rval`
 */
integer integer::operator*(const integer &rval) {
	integer r;

	multiply(*this, rval, &r);

	return(r);
}

/*
 *	integer& integer::operator/(const integer &rval)
 *
 *	Overload operator `r = [ptr: this] / @rval`
 */
integer integer::operator/(const integer &rval) {
	integer r;

	divide(*this, rval, &r, NULL);

	return(r);
}

/*
 *	integer& integer::operator%(const integer &rval)
 *
 *	Overload operator `[ptr: this] % @rval`
 */
integer integer::operator%(const integer &rval) {
	integer r, mod;

	divide(*this, rval, &r, &mod);

	return(mod);
}

/*
 *	integer& integer::operator++()
 *
 *	Overload operator `++[ptr: this]`
 */
integer& integer::operator++() {
	integer r, one;

	one.setValue(1);
	plus(*this, one, &r);
	*this = r;

	return(*this);
}

/*
 *	integer integer::operator++(int)
 *
 *	Overload operator `[ptr: this]++`
 */
integer integer::operator++(int) {
	integer r, one, old;

	old = *this;
	one.setValue(1);
	plus(*this, one, &r);
	*this = r;

	return(old);
}

/*
 *	integer& integer::operator--()
 *
 *	Overload operator `--[ptr: this]`
 */
integer& integer::operator--() {
	integer r, one;

	one.setValue(1);
	minus(*this, one, &r);
	*this = r;

	return(*this);
}

/*
 *	integer integer::operator--(int)
 *
 *	Overload operator `[ptr: this]--`
 */
integer integer::operator--(int) {
	integer r, one, old;

	old = *this;
	one.setValue(1);
	minus(*this, one, &r);
	*this = r;

	return(old);
}

/*
 *	integer& integer::operator+=(const integer &rval)
 *
 *	Overload operator `[ptr: this] += @rval`
 */
integer& integer::operator+=(const integer &rval) {
	integer r;

	plus(*this, rval, &r);
	*this = r;

	return(*this);
}

/*
 *	integer& integer::operator-=(const integer &rval)
 *
 *	Overload operator `[ptr: this] -= @rval`
 */
integer& integer::operator-=(const integer &rval) {
	integer r;

	minus(*this, rval, &r);
	*this = r;

	return(*this);
}

/*
 *	integer& integer::operator*=(const integer &rval)
 *
 *	Overload operator `[ptr: this] *= @rval`
 */
integer& integer::operator*=(const integer &rval) {
	integer r;

	multiply(*this, rval, &r);
	*this = r;

	return(*this);
}

/*
 *	integer& integer::operator/=(const integer &rval)
 *
 *	Overload operator `[ptr: this] /= @rval`
 */
integer& integer::operator/=(const integer &rval) {
	integer r;

	divide(*this, rval, &r, NULL);
	*this = r;

	return(*this);
}

/*
 *	integer& integer::operator%=(const integer &rval)
 *
 *	Overload operator `[ptr: this] %= @rval`
 */
integer& integer::operator%=(const integer &rval) {
	integer r, mod;

	divide(*this, rval, &r, &mod);
	*this = mod;

	return(*this);
}

/*
 *	integer gcd(const integer &na, const integer &nb)
 *
 *	Get the greatest common divisor of @na and @nb.
 */
integer gcd(const integer &na, const integer &nb) {
	integer t, a(na), b(nb);

	while(b != static_cast<integer>(0)) {
		t = a % b;
		a = b;
		b = t;
	}

	return(a);
}

/*
 *	integer lcm(const integer &na, const integer &nb)
 *
 *	Get the lease common multiple of @na and @nb.
 */
integer lcm(const integer &na, const integer &nb) {
	return((const_cast<integer&>(na) * const_cast<integer&>(nb)) / gcd(na, nb));
}

/*
 *	integer int_to_integer(const int src)
 *
 *	External function, it's similar to 'setValue(int)'.
 */
integer int_to_integer(const int src) {
	integer r(src);
	return(r);
}

