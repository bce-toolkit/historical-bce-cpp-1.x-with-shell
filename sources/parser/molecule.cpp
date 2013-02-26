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
#include <vector>
#include <iterator>
#include <algorithm>
#include <math/integer.hpp>
#include <parser/bracket.hpp>
#include <parser/common.hpp>
#include <parser/element.hpp>

#define __REQUIRE_MOLECULE_REPLACE_TABLE__
#include <parser/molecule.hpp>
#undef __REQUIRE_MOLECULE_REPLACE_TABLE__

using namespace std;

/*
 *	bool findHydrateDot(const string &formula, size_t *position)
 *
 *	Find the first hydrate dot in the formula.
 */
bool findHydrateDot(const string &formula, size_t *position) {
	size_t idx;
	integer stack;
	stack.setValue(0);
	for (idx = 0; idx < formula.length(); idx++) {
		if (formula.substr(idx, 1) == BRACKET_START_A || formula.substr(idx, 1) == BRACKET_START_B || formula.substr(idx, 1) == BRACKET_START_C) {
			stack++;
			continue;
		}

		if (formula.substr(idx, 1) == BRACKET_END_A || formula.substr(idx, 1) == BRACKET_END_B || formula.substr(idx, 1) == BRACKET_END_C) {
			stack--;
			continue;
		}

		if (stack.isZero() == true && formula.substr(idx, 1) == PARSER_MOLECULE_HYDRATE_SIGN) {
			if (position != NULL) {
				*position = idx;
			}
			return(true);
		}
	}
	return(false);
}

/*
 *	bool parseMolecule(const string &formula, int *status, const integer &suffix, vector<element> &result)
 *
 *	Parse a chemical formula.
 */
bool parseMolecule(const string &formula, int *status, const integer &suffix, vector<element> &result) {
	size_t idx, bkbegin, bkend, tableID;
	integer rdsuffix, bksuffix, stack;
	element build;
	string rdformula, rdprefix, bkleft, bkmid, bkright, tmp_prefix, tmp_formula;
	vector<element>::iterator find;

	/*  Separate with "."  */
	if (findHydrateDot(formula, &idx) == true) {
		return(parseMolecule(formula.substr(idx + 1), status, suffix, result) == true && parseMolecule(formula.substr(0, idx), status, suffix, result) == true);
	}

	/*  Get the prefix number  */
	rdprefix = parserNumericPrefix(formula, rdformula);
	if (rdprefix.length() == 0) {
		rdprefix = PARSER_MOLECULE_NUMBER_ONE;
	}

	rdsuffix.setValue(rdprefix);
	rdsuffix *= suffix;
	if (rdformula.length() == 0) {
		return(true);
	}

	/*  Parse bracket part  */
	stack.setValue(0);
	for (bkbegin = 0; bkbegin < rdformula.length(); bkbegin++) {
		if (rdformula.substr(bkbegin, 1) == BRACKET_START_A || rdformula.substr(bkbegin, 1) == BRACKET_START_B || rdformula.substr(bkbegin, 1) == BRACKET_START_C) {
			for (idx = bkbegin; idx < rdformula.length(); idx++) {
				if (rdformula.substr(idx, 1) == BRACKET_START_A || rdformula.substr(idx, 1) == BRACKET_START_B || rdformula.substr(idx, 1) == BRACKET_START_C) {
					stack++;
					continue;
				}

				if (rdformula.substr(idx, 1) == BRACKET_END_A || rdformula.substr(idx, 1) == BRACKET_END_B || rdformula.substr(idx, 1) == BRACKET_END_C) {
					stack--;
					if (stack.isZero() == true) {
						bkend = idx;
						goto bracket_analyzed;
					}
					continue;
				}
			}
			return(false);

bracket_analyzed:
			/*  Left part  */
			bkleft = rdformula.substr(0, bkbegin);

			/*  Bracket part  */
			bkmid = rdformula.substr(bkbegin + 1, bkend - bkbegin - 1);

			/*  Right part  */
			bkright = rdformula.substr(bkend + 1);

			/*  Get the suffix of the bracket  */
			tmp_prefix = parserNumericPrefix(bkright, tmp_formula);
			if (tmp_prefix.length() == 0) {
				tmp_prefix = PARSER_MOLECULE_NUMBER_ONE;
			}
			bksuffix.setValue(tmp_prefix);
			bkright = tmp_formula;

			/*  Parse three parts  */
			return(parseMolecule(bkleft, status, rdsuffix, result) == true && parseMolecule(bkmid, status, rdsuffix * bksuffix, result) == true && parseMolecule(bkright, status, rdsuffix, result) == true);
		}
	}
	/*  Analyze elements  */
	for (idx = 1; idx <= rdformula.length(); idx++) {
		if (idx == rdformula.length() || isupper(rdformula.at(idx))) {
			/*  Check whether current formula is in the replace table  */
			for (tableID = 0; replace_table[tableID].name.length() != 0; tableID++) {
				if (rdformula.substr(0, tableID) == replace_table[tableID].name) {
					/*  Re-parse  */
					if (parseMolecule(replace_table[tableID].target, status, rdsuffix * replace_table[tableID].suffix, result) == false) {
						return(false);
					}

					if (status != NULL && replace_table[tableID].status != STATUS_NONE) {
						*status = replace_table[tableID].status;
					}

					return(parseMolecule(rdformula.substr(idx), status, rdsuffix, result));
				}
			}

			/*  Parse single element  */
			if (build.parseString(rdformula.substr(0, idx)) == false) {
				return(false);
			}

			build.count *= rdsuffix;

			find = lower_bound(result.begin(), result.end(), build);
			if (find != result.end() && *find == build) {
				find->count += build.count;
				if (find->count.isZero() == true) {
					result.erase(find);
				}
			} else {
				result.insert(find, build);
			}

			return(parseMolecule(rdformula.substr(idx), status, rdsuffix, result));
		}
	}

	return(false);
}

