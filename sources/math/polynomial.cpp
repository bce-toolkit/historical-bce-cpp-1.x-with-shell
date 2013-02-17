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

#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <common/exception.hpp>
#include <math/integer.hpp>
#include <math/fraction.hpp>
#include <math/polynomial_unknown.hpp>
#include <math/polynomial.hpp>

using namespace std;

/*  Internal macro  */
/*  __max(a, b): Get the maximum number of two integers (@a, @b)  */
#ifdef __min
	#undef __min
#endif
#define __min(a,b) ((a) < (b) ? (a) : (b))

/*
 *	polynomial::polynomial()
 *
 *	Constructor.
 */
polynomial::polynomial() {
	unknowns.clear();
	constant.setValue(0, 1);
}

/*
 *	polynomial::polynomial(const polynomial &src)
 *
 *	Copy constructor.
 */
polynomial::polynomial(const polynomial &src) {
	unknowns = src.unknowns;
	constant = src.constant;
}

/*
 *	polynomial::polynomial(const fraction &src)
 *
 *	Construct from type 'fraction'.
 */
polynomial::polynomial(const fraction &src) {
	unknowns.clear();
	constant = src;
}

/*
 *	polynomial::~polynomial()
 *
 *	Destructor.
 */
polynomial::~polynomial() {
	/*  Dummy destructor  */
}

/*
 *	bool polynomial::isNumeric()
 *
 *	Get whether the polynomial is a real number.
 */
bool polynomial::isNumeric() {
	if (unknowns.size() == 0) {
		return(true);
	} else {
		return(false);
	}
}

/*
 *	void polynomial::clear()
 *
 *	Remove all unknowns.
 */
void polynomial::clear() {
	unknowns.clear();
}

/*
 *	void polynomial::addUnknown(const integer &sign, const fraction &prefix)
 *
 *	Add an unknown to the polynomial if it doesn't exist. Otherwise, add its prefix
 *	upto the existed one.
 */
void polynomial::addUnknown(const integer &sign, const fraction &prefix) {
	polynm_unknown newItem;
	vector<polynm_unknown>::iterator find;

	if (const_cast<fraction&>(prefix).isZero() == true) {
		return;
	}

	newItem.setValue(sign, prefix);

	find = lower_bound(unknowns.begin(), unknowns.end(), newItem);
	if (find != unknowns.end()) {
		if (*find == newItem) {
			find->prefix += newItem.prefix;
			if (find->prefix.isZero() == true) {
				unknowns.erase(find);
			}
			return;
		}
	}
	unknowns.insert(find, newItem);
}

/*
 *	fraction polynomial::getConstant()
 *
 *	Get constant part of the polynomial.
 */
fraction polynomial::getConstant() {
	return(constant);
}

/*
 *	fraction polynomial::setConstant()
 *
 *	Set constant part of the polynomial.
 */
void polynomial::setConstant(const fraction &src) {
	constant = src;
}

/*
 *	void polynomial::multiply(const fraction &src)
 *
 *	Multiply the polynomial by a fraction.
 */
void polynomial::multiply(const fraction &src) {
	vector<polynm_unknown>::iterator iterUnknown;

	constant *= src;

multiply:
	for (iterUnknown = unknowns.begin(); iterUnknown != unknowns.end(); iterUnknown++) {
		iterUnknown->prefix *= src;
		if (iterUnknown->prefix.isZero() == true) {
			unknowns.erase(iterUnknown);
			goto multiply;
		}
	}
}

/*
 *	void polynomial::multiply(const fraction &src)
 *
 *	Multiply the polynomial by a 'int'.
 */
void polynomial::multiply(const int &src) {
	multiply(fraction(src, 1));
}

/*
 *	void polynomial::merge(const polynomial &src)
 *
 *	Merge another polynomial to this polynomial.
 */
void polynomial::merge(const polynomial &src) {
	vector<polynm_unknown>::iterator iterUnknown;

	constant += src.constant;

	for (iterUnknown = const_cast<polynomial&>(src).unknowns.begin(); iterUnknown != const_cast<polynomial&>(src).unknowns.end(); iterUnknown++) {
		addUnknown(iterUnknown->sign, iterUnknown->prefix);
	}
}

/*
 *	void polynomial::mergeCost(const polynomial &src)
 *
 *	Merge another polynomial to this polynomial (but use minus operation when an unknown existed).
 */
void polynomial::mergeCost(const polynomial &src) {
	vector<polynm_unknown>::iterator iterUnknown;

	constant -= src.constant;

	for (iterUnknown = const_cast<polynomial&>(src).unknowns.begin(); iterUnknown != const_cast<polynomial&>(src).unknowns.end(); iterUnknown++) {
		addUnknown(iterUnknown->sign, fraction(0, 1) - iterUnknown->prefix);
	}
}

/*
 *	void polynomial::vectorAllToInteger(vector<polynomial> &dest)
 *
 *	Set all prefix of unknowns to integer.
 */
void polynomial::vectorAllToInteger(vector<polynomial> &dest) {
	vector<polynomial>::iterator iterDest;
	vector<polynm_unknown>::iterator iterUnknown;
	integer mp(1);

	for (iterDest = dest.begin(); iterDest != dest.end(); iterDest++) {
		for (iterUnknown = iterDest->unknowns.begin(); iterUnknown != iterDest->unknowns.end(); iterUnknown++) {
			mp = lcm(iterUnknown->prefix.getDenominator(), mp);
		}
		mp = lcm(iterDest->getConstant().getDenominator(), mp);
	}

	for (iterDest = dest.begin(); iterDest != dest.end(); iterDest++) {
		for (iterUnknown = iterDest->unknowns.begin(); iterUnknown != iterDest->unknowns.end(); iterUnknown++) {
			iterUnknown->prefix *= fraction(mp, integer(1));
		}
		iterDest->setConstant(iterDest->getConstant() * fraction(mp, integer(1)));
	}
}

/*
 *	void polynomial::vectorRemoveOnlyUnknown(vector<polynomial> &dest)
 *
 *	Remove the only unknown if a polynomial vector(set) only have one unknown.
 */
void polynomial::vectorRemoveOnlyUnknown(vector<polynomial> &dest) {
	vector<polynomial>::iterator iterDest;
	vector<polynm_unknown>::iterator iterUnknown;

	for (iterDest = dest.begin(); iterDest != dest.end(); iterDest++) {
		if (iterDest->unknowns.size() != 1 || iterDest->unknowns.at(0).sign.isZero() == false) {
			return;
		}
	}

	for (iterDest = dest.begin(); iterDest != dest.end(); iterDest++) {
		iterDest->setConstant( iterDest->getConstant() + iterDest->unknowns.at(0).prefix                                  );
		iterDest->unknowns.clear();
	}
}

/*
 *	void polynomial::vectorReindex(vector<polynomial> &dest)
 *
 *	Arrange the vector of polynomials and make their prefix number consecutively.
 *
 *	FIXME: This function needs to be declared as a global function, 
 *	       but it has to visit some private members of this class.
 *	       No matter how, it's not a good idea to declare it as a 
 *	       public function in a class.
 */
void polynomial::vectorReindex(vector<polynomial> &dest) {
	size_t idx, *next;
	bool loadedID;
	integer allocID(0), minID(0);
	do {
		next = new size_t[dest.size()]();
	} while(next == NULL);

	while(true) {
		loadedID = false;
		for (idx = 0; idx < dest.size(); idx++) {
			if (next[idx] != dest.at(idx).unknowns.size()) {
				if (loadedID == false) {
					minID = dest.at(idx).unknowns.at(next[idx]).sign;
				} else {
					if (dest.at(idx).unknowns.at(next[idx]).sign < minID) {
						minID = dest.at(idx).unknowns.at(next[idx]).sign;
					}
				}
				loadedID = true;
			}
		}
		if (loadedID == false) {
			break;
		}
		for (idx = 0; idx < dest.size(); idx++) {
			if (next[idx] != dest.at(idx).unknowns.size()) {
				if (dest.at(idx).unknowns.at(next[idx]).sign == minID) {
					dest.at(idx).unknowns.at(next[idx]).sign = allocID;
					next[idx]++;
				}
			}
		}
		allocID++;
	}
	delete []next;
}

/*
 *	string polynomial::toString()
 *
 *	Convert the polynomial to string.
 */
string polynomial::toString() {
	string ret = "", tmp = "", scst;
	vector<polynm_unknown>::iterator iterUnknown;

	for (iterUnknown = unknowns.begin(); iterUnknown != unknowns.end(); iterUnknown++) {
		tmp = iterUnknown->toString();

		if (ret.length() != 0 && ret.at(ret.length() - 1) == '+' && tmp.substr(0, 1) == POLYNOMIAL_MINUS) {
			ret.erase(ret.length() - 1, 1);
		}

		if (tmp.length() != 0) {
			ret += tmp + ((iterUnknown + 1 == unknowns.end()) ? "" : POLYNOMIAL_PLUS);
		}
	}

	scst = constant.toString();

	if (scst != integer(0).toString()) {
		if (scst.substr(0, 1) == POLYNOMIAL_MINUS) {
			ret += scst;
		} else {
			ret += ((ret.length() == 0) ? "" : POLYNOMIAL_PLUS) + scst;
		}
	}

	if (ret.length() == 0) {
		ret = integer(0).toString();
	}

	return(ret);
}

/*
 *	bool polynomial::isNoUnknown()
 *
 *	Get whether the unknown vector is empty.
 */
bool polynomial::isNoUnknown() {
	return(unknowns.size() == 0 ? true : false);
}

/*
 *	bool polynomial::isAllNegative()
 *
 *	Get whether all unknowns and constant were negative.
 */
bool polynomial::isAllNegative() {
	vector<polynm_unknown>::iterator iterUnknown;

	for (iterUnknown = unknowns.begin(); iterUnknown != unknowns.end(); iterUnknown++) {
		if (iterUnknown->prefix.isNegative() == false) {
			return(false);
		}
	}

	if (constant.isNegative() == false && constant.isZero() == false) {
		return(false);
	} else {
		return(true);
	}
}

/*
 *	polynomial& polynomial::operator=(const polynomial &src)
 *
 *	Overload operator `[ptr: this] = @src`
 */
polynomial& polynomial::operator=(const polynomial &src) {
	if (this == &src) {
		return(*this);
	}

	unknowns = src.unknowns;
	constant = src.constant;

	return(*this);
}

