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

#include <common/exception.hpp>
#include <math/integer.hpp>
#include <math/fraction.hpp>
#include <math/polynomial_unknown.hpp>

using namespace std;

/*
 *	t_polynm_unknown::t_polynm_unknown()
 *
 *	Constructor.
 */
t_polynm_unknown::t_polynm_unknown() {
	sign = integer(-1);
	prefix.setValue(0, 1);
}

/*
 *	t_polynm_unknown::t_polynm_unknown(const struct t_polynm_unknown &src)
 *
 *	Copy constructor.
 */
t_polynm_unknown::t_polynm_unknown(const struct t_polynm_unknown &src) {
	setValue(src.sign, src.prefix);
}

/*
 *	t_polynm_unknown::~t_polynm_unknown()
 *
 *	Destructor.
 */
t_polynm_unknown::~t_polynm_unknown() {
	/*  Dummy destructor  */
}

/*
 *	void t_polynm_unknown::setValue(const integer &src_sign, const fraction &src_prefix)
 *
 *	Set internal values.
 */
void t_polynm_unknown::setValue(const integer &src_sign, const fraction &src_prefix) {
	sign = src_sign;
	prefix = src_prefix;
}

/*
 *	string t_polynm_unknown::unknownName(integer id)
 *
 *	Get the symbol by using its id.
 */
string t_polynm_unknown::unknownName(const integer &id) {
	string ret = "";
	integer tmp(id);
	const char *unknown_table = "abcdefghijklmnopqrstuvwxyz";

	#define unknown_table_length 26

	do {
		ret.insert(0, 1, unknown_table[static_cast<size_t>((tmp % integer(unknown_table_length)).toInt())]);
		tmp /= integer(unknown_table_length);
	} while (tmp != 0);

	return(ret);
}

/*
 *	string t_polynm_unknown::toString()
 *
 *	Convert the unknown to string.
 */
string t_polynm_unknown::toString() {
	string ret = "";

	if (sign.isNegative() == true) {
		throw xsvException(ERROR_UNREACHABLE, __FILE__, __LINE__, __FUNCTION__, "Invalid unknown sign.");
	}

	if (prefix.isZero() == true) {
		throw xsvException(ERROR_DIVIDE_ZERO, __FILE__, __LINE__, __FUNCTION__, "Unknown prefix should be zero.");
	}

	ret += prefix.toString();

	if (ret == integer(1).toString()) {
		ret = "";
	}

	if (ret == integer(-1).toString()) {
		ret = INTEGER_NEGATIVE_STRING;
	}

	ret += POLYNM_UNKNOWN_SYMBOL_PREFIX + unknownName(sign);

	return(ret);
}

/*
 *	t_polynm_unknown& t_polynm_unknown::operator=(const t_polynm_unknown &src)
 *
 *	Overload operator `[ptr: this] = @src`
 */
struct t_polynm_unknown& t_polynm_unknown::operator=(const t_polynm_unknown &src) {
	if (this == &src) {
		return(*this);
	}

	setValue(src.sign, src.prefix);

	return(*this);
}

/*
 *	bool t_polynm_unknown::operator==(const struct t_polynm_unknown &src)
 *
 *	Overload operator `[ptr: this] == @src`
 */
bool t_polynm_unknown::operator==(const struct t_polynm_unknown &src) {
	if (this->sign == src.sign) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool t_polynm_unknown::operator<(const struct t_polynm_unknown &src)
 *
 *	Overload operator `[ptr: this] < @src`
 */
bool t_polynm_unknown::operator<(const struct t_polynm_unknown &src) {
	if (this->sign < src.sign) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool t_polynm_unknown::operator<=(const struct t_polynm_unknown &src)
 *
 *	Overload operator `[ptr: this] <= @src`
 */
bool t_polynm_unknown::operator<=(const struct t_polynm_unknown &src) {
	if (this->sign <= src.sign) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool t_polynm_unknown::operator>(const struct t_polynm_unknown &src)
 *
 *	Overload operator `[ptr: this] > @src`
 */
bool t_polynm_unknown::operator>(const struct t_polynm_unknown &src) {
	if (this->sign > src.sign) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool t_polynm_unknown::operator>=(const struct t_polynm_unknown &src)
 *
 *	Overload operator `[ptr: this] >= @src`
 */
bool t_polynm_unknown::operator>=(const struct t_polynm_unknown &src) {
	if (this->sign >= src.sign) {
		return(true);
	} else {
		return(false);
	}
}

