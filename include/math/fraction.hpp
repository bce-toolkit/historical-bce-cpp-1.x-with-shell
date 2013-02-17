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

#ifndef __FRACTION_HPP__
	#define __FRACTION_HPP__

	#include <string>
	#include <math/integer.hpp>

	#define FRACTION_SEMICOLON "/"

	using namespace std;

	class fraction {
	public:
		/*  Public declares  */
		fraction();
		fraction(const fraction &src);
		fraction(const integer &numer, const integer &denomin);
		fraction(const int &numer, const int &denomin);
		~fraction();
		string toString();
		integer getNumerator();
		integer getDenominator();
		void setValue(const fraction &src);
		void setValue(const integer &numer, const integer &denomin);
		void setValue(const int &numer, const int &denomin);
		bool isZero();
		bool isNegative();
		fraction& operator=(const fraction &src);
		bool operator==(const fraction &src);
		bool operator!=(const fraction &src);
		bool operator<(const fraction &src);
		bool operator<=(const fraction &src);
		bool operator>(const fraction &src);
		bool operator>=(const fraction &src);
		fraction operator+(const fraction &rval);
		fraction operator-(const fraction &rval);
		fraction operator*(const fraction &rval);
		fraction operator/(const fraction &rval);
		fraction& operator+=(const fraction &rval);
		fraction& operator-=(const fraction &rval);
		fraction& operator*=(const fraction &rval);
		fraction& operator/=(const fraction &rval);
	private:
		/*  Numerator & denominator  */
		integer numerator, denominator;

		/*  Private declares  */
		void simplify();
		fraction plus(const fraction &na, const fraction &nb);
		fraction minus(const fraction &na, const fraction &nb);
		fraction multiply(const fraction &na, const fraction &nb);
		fraction divide(const fraction &na, const fraction &nb);
		int compare(const fraction &na, const fraction &nb);
	};
#endif

