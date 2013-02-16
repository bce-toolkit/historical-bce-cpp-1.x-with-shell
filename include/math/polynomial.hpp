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

#ifndef __POLYNOMIAL_H__
	#define __POLYNOMIAL_H__

	#include <string>
	#include <vector>
	#include <math/integer.hpp>
	#include <math/fraction.hpp>
	#include <math/polynomial_unknown.hpp>

	using std::vector;
	using std::string;

	class polynomial {
	public:
		/*  Public declares  */
		polynomial();
		polynomial(const polynomial &src);
		polynomial(const fraction &src);
		~polynomial();
		bool isNumeric();
		void clear();
		void sortOut();
		void addUnknown(const integer &sign, const fraction &prefix);
		fraction getConstant();
		void setConstant(const fraction &src);
		void multiply(const fraction &src);
		void multiply(const int &src);
		void merge(const polynomial &src);
		void mergeCost(const polynomial &src);
		void vectorAllToInteger(vector<polynomial> &dest);
		void vectorRemoveOnlyUnknown(vector<polynomial> &dest);
		void vectorReindex(vector<polynomial> &dest);
		string toString();
		bool isNoUnknown();
		bool isAllNegative();
		polynomial& operator=(const polynomial &src);
	private:
		vector<polynm_unknown> unknowns;
		fraction constant;
	};

	/*  External  */
	#define POLYNOMIAL_PLUS "+"
	#define POLYNOMIAL_MINUS "-"
	#define POLYNOMIAL_BRACKET_BEGIN "("
	#define POLYNOMIAL_BRACKET_END ")"
	#define POLYNOMIAL_ZERO "0"
#endif

