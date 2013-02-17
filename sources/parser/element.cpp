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

#include <cctype>
#include <string>
#include <parser/common.hpp>
#include <parser/element.hpp>

using namespace std;

/*
 *	t_element::t_element()
 *
 *	Constructor.
 */
t_element::t_element() {
	symbol = "";
	count.setValue(0);
}

/*
 *	t_element::t_element(const t_element &src)
 *
 *	Copy constructor.
 */
t_element::t_element(const t_element &src) {
	setValue(src.symbol, src.count);
}

/*
 *	t_element::t_element(const string &src_symbol, const integer &src_count)
 *
 *	Construct from specified values.
 */
t_element::t_element(const string &src_symbol, const integer &src_count) {
	symbol = src_symbol;
	count = src_count;
}

/*
 *	t_element::~t_element()
 *
 *	Destructor.
 */
t_element::~t_element() {
	/*  Dummy destructor  */
}

/*
 *	void t_element::setValue(const string &src_symbol, const integer &src_count)
 *
 *	Set values.
 */
void t_element::setValue(const string &src_symbol, const integer &src_count) {
	symbol = src_symbol;
	count = src_count;
}

/*
 *	bool t_element::parseString(const string &src)
 *
 *	Set values from a string([Symbol][Count]) like "Na2".
 */
bool t_element::parseString(const string &src) {
	string t_number;
	t_number = parserNumericSuffix(src, symbol);

	if (symbol.length() == 0) {
		return(false);
	}

	if (t_number.length() == 0) {
		count.setValue(1);
	} else {
		count.setValue(t_number);
	}

	if (count.isZero() == true) {
		return(false);
	} else {
		return(true);
	}
}
/*
 *	struct t_element& t_element::operator=(const struct t_element &src)
 *
 *	Overload operator `[ptr: this] = @src`
 */
struct t_element& t_element::operator=(const struct t_element &src) {
	if (this == &src) {
		return(*this);
	}

	setValue(src.symbol, src.count);

	return(*this);
}

/*
 *	bool t_element::operator==(const struct t_element &src)
 *
 *	Overload operator `[ptr: this] == @src`
 */
bool t_element::operator==(const struct t_element &src) {
	if (this->symbol == src.symbol) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool t_element::operator<(const struct t_element &src)
 *
 *	Overload operator `[ptr: this] < @src`
 */
bool t_element::operator<(const struct t_element &src) {
	if (this->symbol < src.symbol) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool t_element::operator<=(const struct t_element &src)
 *
 *	Overload operator `[ptr: this] <= @src`
 */
bool t_element::operator<=(const struct t_element &src) {
	if (this->symbol <= src.symbol) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool t_element::operator>(const struct t_element &src)
 *
 *	Overload operator `[ptr: this] > @src`
 */
bool t_element::operator>(const struct t_element &src) {
	if (this->symbol > src.symbol) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	bool t_element::operator>=(const struct t_element &src)
 *
 *	Overload operator `[ptr: this] >= @src`
 */
bool t_element::operator>=(const struct t_element &src) {
	if (this->symbol >= src.symbol) {
		return(true);
	} else {
		return(false);
	}
}

