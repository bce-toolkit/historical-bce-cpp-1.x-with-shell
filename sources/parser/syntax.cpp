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
#include <common/exception.hpp>
#include <math/integer.hpp>
#include <math/fraction.hpp>
#include <math/matrix.hpp>
#include <math/polynomial.hpp>
#include <math/equation.hpp>
#include <parser/bracket.hpp>
#include <parser/common.hpp>
#include <parser/element.hpp>
#include <parser/molecule.hpp>
#include <parser/syntax.hpp>

using std::string;
using std::vector;
using std::iterator;

/*
 *	vector<cdec> syntaxPreparser(const string formula)
 *
 *	Pre-parse a chemical equation and separate it into chemical formulas.
 */
vector<cdec> syntaxPreparser(const string formula) {
	size_t idx, last;
	bool rdequal, rdnegative;
	string cheq, remainSymbol;
	vector<cdec> r;
	integer stack;
	cdec build;

	/*  Clear  */
	r.clear();
	stack.setValue(0);
	last = 0;
	rdequal = false;

	for (idx = 0; idx <= formula.length(); idx++) {
		if (idx < formula.length()) {
			if (formula.substr(idx, 1) == BRACKET_START_A || formula.substr(idx, 1) == BRACKET_START_B || formula.substr(idx, 1) == BRACKET_START_C) {
				stack++;
				continue;
			}

			if (formula.substr(idx, 1) == BRACKET_END_A || formula.substr(idx, 1) == BRACKET_END_B || formula.substr(idx, 1) == BRACKET_END_C) {
				stack--;
				continue;
			}
		}

		if (stack != integer(0)) {
			continue;
		}

		/*  Analyze the chemical formula of the chemical equation  */
		if (idx == formula.length() || formula.substr(idx, 1) == PARSER_SYNTAX_PLUS || formula.substr(idx, 1) == PARSER_SYNTAX_MINUS || formula.substr(idx, 1) == PARSER_SYNTAX_SEPARATOR || formula.substr(idx, 1) == PARSER_SYNTAX_EQUAL) {
			/*  Get the chemical formula  */
			cheq = formula.substr(last, idx - last);
			if (cheq == "") {
				if (formula.substr(idx, 1) == PARSER_SYNTAX_EQUAL) {
					rdequal = true;
				}

				last = idx + 1;

				continue;
			}

			/*  Decide whether the connector before a chemical is PARSER_SYNTAX_MINUS  */
			if (last == 0) {
				rdnegative = false;
			} else {
				rdnegative = (formula.substr(last - 1, 1) == PARSER_SYNTAX_MINUS ? true : false);
			}

			/*
			 *  Remove existed prefix, otherwise an issue will occured:
			 *  Input: 3C+O2=CO
			 *  Wrong output: 23C+3O2=6CO
			 *  Right output: 2C+O2=2CO
			 */
			parserNumericPrefix(cheq, remainSymbol);

			/*  Insert the parsed one to result vector  */
			build.symbol = remainSymbol;
			build.negative = rdnegative;
			build.right = rdequal;
			r.insert(r.end(), build);

			if (formula.substr(idx, 1) == PARSER_SYNTAX_EQUAL) {
				rdequal = true;
			}

			last = idx + 1;
		}
	}
	return(r);
}

/*
 *	bool syntaxDecodedToMatrix(vector<cdec> &cdecTable, matrix &target, size_t &mx, size_t &my)
 *
 *	Convert a parsed chemical equation to a matrix.
 */
bool syntaxDecodedToMatrix(vector<cdec> &cdecTable, matrix &target, size_t &mx, size_t &my) {
	size_t write_x, write_y;
	vector<cdec>::iterator iterTable;
	vector<element> sampleElements, parsedElements;
	vector<element>::iterator iterElement1, iterElement2;
	matrix r;

	/*  Clear  */
	sampleElements.clear();
	r.clear();

	/*  Pre-parse existed elements  */
	for (iterTable = cdecTable.begin(); iterTable != cdecTable.end(); iterTable++) {
		if (parseMolecule(iterTable->symbol, NULL, 1, sampleElements) == false) {
			return(false);
		}
	}

	/*  Decide and reset the matrix size  */
	mx = cdecTable.size() + 1;
	my = sampleElements.size();

	if (mx <= 1 || my == 0) {
		return(false);
	}

	r.resizeMatrix(cdecTable.size() + 1, sampleElements.size());

	/*  Re-parse and write data to the matrix  */
	for (iterTable = cdecTable.begin(), write_x = 0; iterTable != cdecTable.end(); iterTable++, write_x++) {
		parsedElements.clear();
		if (parseMolecule(iterTable->symbol, NULL, integer(iterTable->negative == iterTable->right ? 1 : -1), parsedElements) == false) {
			return(false);
		}

		for (iterElement1 = parsedElements.begin(); iterElement1 != parsedElements.end(); iterElement1++) {
			for (iterElement2 = sampleElements.begin(), write_y = 0; iterElement2 != sampleElements.end(); iterElement2++, write_y++) {
				if (*iterElement1 == *iterElement2) {
					r.write(write_x, write_y, fraction(iterElement1->count, integer(1)));
					break;
				}
			}
		}
	}

	target = r;

	return(true);
}

