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
#include <string>
#include <common/exception.hpp>
#include <math/integer.hpp>
#include <math/fraction.hpp>

using namespace std;

/*
 *	fraction::fraction()
 *
 *	Constructor.
 */
fraction::fraction() {
	numerator.setValue(0);
	denominator.setValue(1);
}

/*
 *	fraction::fraction(const fraction &src)
 *
 *	Copy constructor.
 */
fraction::fraction(const fraction &src) {
	numerator = src.numerator;
	denominator = src.denominator;
}

/*
 *	fraction::fraction(const integer &numer, const integer &denomin)
 *
 *	Construct from type 'integer'.
 */
fraction::fraction(const integer &numer, const integer &denomin) {
	setValue(numer, denomin);
}

/*
 *	fraction::fraction(const int &numer, const int &denomin)
 *
 *	Construct from type 'int'.
 */
fraction::fraction(const int &numer, const int &denomin) {
	setValue(numer, denomin);
}

/*
 *	fraction::~fraction()
 *
 *	Destructor.
 */
fraction::~fraction() {
	/*  Dummy destructor  */
}

/*
 *	string fraction::toString()
 *
 *	Convert the integer to type 'string'.
 */
string fraction::toString() {
	integer tmp;

	if ((numerator % denominator).isZero() == true) {
		return((numerator / denominator).toString());
	}

	return(numerator.toString() + FRACTION_SEMICOLON + denominator.toString());
}

/*
 *	void fraction::simplify()
 *
 *	Simplify the fraction and adjust the negative sign of its numerator and denominator.
 */
void fraction::simplify() {
	integer g;

	if (numerator.isZero() == true) {
		denominator = integer(1);
		return;
	}
	
	/*  Change workspace  */
	g = gcd(numerator, denominator);
	numerator /= g;
	denominator /= g;

	if (denominator.isNegative() == true) {
		numerator.opposite();
		denominator.absolute();
	}
}

/*
 *	integer fraction::getNumerator()
 *
 *	Get the numerator part of the fraction.
 */
integer fraction::getNumerator() {
	return(numerator);
}

/*
 *	integer fraction::getDenominator()
 *
 *	Get the numerator part of the fraction.
 */
integer fraction::getDenominator() {
	return(denominator);
}

/*
 *	void fraction::setValue(const fraction &src)
 *
 *	Set the fraction value from another fraction.
 */
void fraction::setValue(const fraction &src) {
	numerator = src.numerator;
	denominator = src.denominator;
}

/*
 *	void fraction::setValue(const integer &numer, const integer &denomin)
 *
 *	Set the fraction value from type 'integer'.
 */
void fraction::setValue(const integer &numer, const integer &denomin) {
	numerator = numer;
	denominator = denomin;
	simplify();
}

/*
 *	void fraction::setValue(const int &numer, const int &denomin)
 *
 *	Set the fraction value from type 'int'.
 */
void fraction::setValue(const int &numer, const int &denomin) {
	numerator.setValue(numer);
	denominator.setValue(denomin);
	simplify();
}

/*
 *	fraction fraction::plus(const fraction &na, const fraction &nb)
 *
 *	Do plus operation `r = @na + @nb`.
 */
fraction fraction::plus(const fraction &na, const fraction &nb) {
	integer na_numer(na.numerator), na_denomin(na.denominator), nb_numer(nb.numerator), nb_denomin(nb.denominator);
	fraction r(na_numer * nb_denomin + na_denomin * nb_numer, na_denomin * nb_denomin);
	return(r);
}

/*
 *	fraction fraction::minus(const fraction &na, const fraction &nb)
 *
 *	Do plus operation `r = @na - @nb`.
 */
fraction fraction::minus(const fraction &na, const fraction &nb) {
	integer na_numer(na.numerator), na_denomin(na.denominator), nb_numer(nb.numerator), nb_denomin(nb.denominator);
	fraction r(na_numer * nb_denomin - na_denomin * nb_numer, na_denomin * nb_denomin);
	return(r);
}

/*
 *	fraction fraction::multiply(const fraction &na, const fraction &nb)
 *
 *	Do plus operation `r = @na * @nb`.
 */
fraction fraction::multiply(const fraction &na, const fraction &nb) {
	integer na_numer(na.numerator), na_denomin(na.denominator), nb_numer(nb.numerator), nb_denomin(nb.denominator);
	fraction r(na_numer * nb_numer, na_denomin * nb_denomin);
	return(r);
}

/*
 *	fraction fraction::divide(const fraction &na, const fraction &nb)
 *
 *	Do plus operation `r = @na / @nb`.
 */
fraction fraction::divide(const fraction &na, const fraction &nb) {
	integer na_numer(na.numerator), na_denomin(na.denominator), nb_numer(nb.numerator), nb_denomin(nb.denominator);

	if (const_cast<integer&>(nb.numerator).isZero() == true) {
		throw xsvException(ERROR_DIVIDE_ZERO, __FILE__, __LINE__, __FUNCTION__, "Divide by zero.");
	}

	return(fraction(na_numer * nb_denomin, na_denomin * nb_numer));
}

/*
 *	int fraction::compare(const fraction &na, const fraction &nb)
 *
 *	Compare two fraction @na and @nb.
 *
 *	Return values:
 *	    -1 => @na < @nb
 *	     0 => @na = @nb
 *	     1 => @na > @nb
 */
int fraction::compare(const fraction &na, const fraction &nb) {
	integer na_numer(na.numerator), na_denomin(na.denominator), nb_numer(nb.numerator), nb_denomin(nb.denominator), cmp;

	cmp = na_denomin * nb_numer - na_numer * nb_denomin;

	if (cmp.isZero() == true) {
		return(0);
	}

	if (cmp.isNegative() == true) {
		return(1);
	} else {
		return(-1);
	}
}

/*
 *	bool fraction::isZero()
 *
 *	Get whether it is zero.
 */
bool fraction::isZero() {
	if (numerator.isZero() == true) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool fraction::isNegative()
 *
 *	Get whether it is negative.
 */
bool fraction::isNegative() {
	if (numerator.isNegative() == true) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	fraction& fraction::operator=(const fraction &src)
 *
 *	Overload operator `[ptr: this] = @src`
 */
fraction& fraction::operator=(const fraction &src) {
	if (this == &src) {
		return(*this);
	}

	setValue(src);

	return(*this);
}

/*
 *	fraction& fraction::operator==(const fraction &src)
 *
 *	Overload operator `[ptr: this] == @src`
 */
bool fraction::operator==(const fraction &src) {
	if (compare(*this, src) == 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	fraction& fraction::operator!=(const fraction &src)
 *
 *	Overload operator `[ptr: this] != @src`
 */
bool fraction::operator!=(const fraction &src) {
	if (compare(*this, src) != 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool fraction::operator<(const fraction &src)
 *
 *	Overload operator `[ptr: this] < @src`
 */
bool fraction::operator<(const fraction &src) {
	if (compare(*this, src) == -1) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool fraction::operator<=(const fraction &src)
 *
 *	Overload operator `[ptr: this] <= @src`
 */
bool fraction::operator<=(const fraction &src) {
	if (compare(*this, src) <= 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool fraction::operator>(const fraction &src)
 *
 *	Overload operator `[ptr: this] > @src`
 */
bool fraction::operator>(const fraction &src) {
	if (compare(*this, src) == 1) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool fraction::operator>=(const fraction &src)
 *
 *	Overload operator `[ptr: this] >= @src`
 */
bool fraction::operator>=(const fraction &src) {
	if (compare(*this, src) >= 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	fraction fraction::operator+(const fraction &rval)
 *
 *	Overload operator `r = [ptr: this] + @rval`
 */
fraction fraction::operator+(const fraction &rval) {
	return(plus(*this, rval));
}

/*
 *	fraction fraction::operator-(const fraction &rval)
 *
 *	Overload operator `r = [ptr: this] - @rval`
 */
fraction fraction::operator-(const fraction &rval) {
	return(minus(*this, rval));
}

/*
 *	fraction fraction::operator*(const fraction &rval)
 *
 *	Overload operator `r = [ptr: this] * @rval`
 */
fraction fraction::operator*(const fraction &rval) {
	return(multiply(*this, rval));
}

/*
 *	fraction fraction::operator/(const fraction &rval)
 *
 *	Overload operator `r = [ptr: this] / @rval`
 */
fraction fraction::operator/(const fraction &rval) {
	return(divide(*this, rval));
}

/*
 *	fraction& fraction::operator+=(const fraction &rval)
 *
 *	Overload operator `[ptr: this] += @rval`
 */
fraction& fraction::operator+=(const fraction &rval) {
	*this = plus(*this, rval);

	return(*this);
}

/*
 *	fraction& fraction::operator-=(const fraction &rval)
 *
 *	Overload operator `[ptr: this] -= @rval`
 */
fraction& fraction::operator-=(const fraction &rval) {
	*this = minus(*this, rval);

	return(*this);
}

/*
 *	fraction& fraction::operator*=(const fraction &rval)
 *
 *	Overload operator `[ptr: this] *= @rval`
 */
fraction& fraction::operator*=(const fraction &rval) {
	*this = multiply(*this, rval);

	return(*this);
}

/*
 *	fraction& fraction::operator/=(const fraction &rval)
 *
 *	Overload operator `[ptr: this] /= @rval`
 */
fraction& fraction::operator/=(const fraction &rval) {
	*this = divide(*this, rval);

	return(*this);
}

