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

#ifndef __INTEGER_HPP__
	#define __INTEGER_HPP__

	#include <string>

	using std::string;

	class integer {
	public:
		/*  Public declares  */
		integer();
		integer(const integer &src);
		integer(const int src);
		integer(const string &src);
		~integer();
		string toString();
		int toInt();
		void print();
		void printLine();
		void setValue(const string &value);
		void setValue(const int value);
		void setValue(const integer &src);
		void absolute();
		void opposite();
		bool isNegative();
		integer& operator=(const integer &src);
		bool operator==(const integer &src);
		bool operator!=(const integer &src);
		bool operator<(const integer &src);
		bool operator<=(const integer &src);
		bool operator>(const integer &src);
		bool operator>=(const integer &src);
		integer operator+(const integer &rval);
		integer operator-(const integer &rval);
		integer operator*(const integer &rval);
		integer operator/(const integer &rval);
		integer operator%(const integer &rval);
		integer& operator++();
		integer operator++(int);
		integer& operator--();
		integer operator--(int);
		integer& operator+=(const integer &rval);
		integer& operator-=(const integer &rval);
		integer& operator*=(const integer &rval);
		integer& operator/=(const integer &rval);
		integer& operator%=(const integer &rval);
	private:
		/*  Number bits  */
		char *bits;

		/*  Number length  */
		size_t length;

		/*  Negative sign  */
		bool negative;

		/*  Private declares  */
		void plus(const integer &na, const integer &nb, integer *dest);
		void minus(const integer &na, const integer &nb, integer *dest);
		void multiply(const integer &na, const integer &nb, integer *dest);
		void divide(const integer &na, const integer &nb, integer *quotient, integer *mod);
		int compare(const integer &na, const integer &nb);
	};

	/*  External  */
	#define INTEGER_NEGATIVE_STRING "-"

	integer gcd(const integer &na, const integer &nb);
	integer lcm(const integer &na, const integer &nb);
	integer int_to_integer(const int src);
#endif

