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
#include <cctype>
#include <iterator>
#include <common/exception.hpp>
#include <math/integer.hpp>
#include <math/fraction.hpp>
#include <math/matrix.hpp>
#include <math/polynomial.hpp>
#include <math/equation.hpp>
#include <parser/common.hpp>
#include <parser/element.hpp>
#include <parser/molecule.hpp>
#include <parser/syntax.hpp>
#include <balancer.hpp>

using namespace std;

/*
 *	void balancerArrangeSide(vector<cdec> &parsed, vector<polynomial> &result)
 *
 *	Reset the dislocated elements.
 *	(e.g: C=CO2+(-1)O2   =>   C+O2=CO2)
 */
void balancerArrangeSide(vector<cdec> &parsed, vector<polynomial> &result) {
	vector<cdec> resultParsed;
	vector<cdec>::iterator iterParsed;
	vector<polynomial> resultR;
	vector<polynomial>::iterator iterEqResult;
	cdec popParsed;
	polynomial popEqResult;
	bool negative;

	/*  Clear  */
	resultParsed.clear();
	resultR.clear();

	/*  Analyze which element should be put to left side (before "=")  */
	for (iterParsed = parsed.begin(), iterEqResult = result.begin(); iterParsed != parsed.end(); iterParsed++, iterEqResult++) {
		negative = iterEqResult->isAllNegative();

		if (iterParsed->right == false && negative == false) {
			resultParsed.push_back(*iterParsed);
			resultR.push_back(*iterEqResult);
		}

		if (iterParsed->right == true && negative == true) {
			popParsed = *iterParsed;
			popEqResult = *iterEqResult;
			popParsed.right = false;
			popEqResult.multiply(fraction(-1, 1));
			resultParsed.push_back(popParsed);
			resultR.push_back(popEqResult);
		}
	}

	/*  Analyze which element should be put to right side (after "=")  */
	for (iterParsed = parsed.begin(), iterEqResult = result.begin(); iterParsed != parsed.end(); iterParsed++, iterEqResult++) {
		negative = iterEqResult->isAllNegative();

		if (iterParsed->right == true && negative == false) {
			resultParsed.push_back(*iterParsed);
			resultR.push_back(*iterEqResult);
		}

		if (iterParsed->right == false && negative == true) {
			popParsed = *iterParsed;
			popEqResult = *iterEqResult;
			popParsed.right = true;
			popEqResult.multiply(fraction(-1, 1));
			resultParsed.push_back(popParsed);
			resultR.push_back(popEqResult);
		}
	}

	parsed = resultParsed;
	result = resultR;
}

/*
 *	bool balancerProcess(const string formula, vector<cdec> &parsed, vector<polynomial> &result)
 *
 *	Balance a chemical equation and return the parsed chemical equation and balanced results.
 */
bool balancerProcess(const string formula, vector<cdec> &parsed, vector<polynomial> &result) {
	matrix eq;
	size_t mx, my;
	vector<cdec> preparsed;
	vector<cdec>::iterator iterParsed;
	vector<polynomial> eqResult;
	vector<polynomial>::iterator iterEqResult;

	/*  Pre-parse the chemical equation  */
	preparsed = syntaxPreparser(formula);
	/*  Convert to matrix  */
	if (syntaxDecodedToMatrix(preparsed, eq, mx, my) == false) {
		return(false);
	}

	/*  Solve the matrix as linear equations  */
	if (equationSolve(eq, 0, 0, mx, my, eqResult) == false) {
		return(false);
	}

	/*  Re-index unknowns in the result vector  */
	eqResult.begin()->vectorReindex(eqResult);

	/*  Check the result we had got  */
	if (equationCheckResult(eq, mx, my, eqResult) == false) {
		return(false);
	}

	/*  Remove zero-prefixed elements in the result vector  */
	for (iterParsed = preparsed.begin(), iterEqResult = eqResult.begin(); iterParsed != preparsed.end(); iterParsed++, iterEqResult++) {
		if (iterEqResult->isNumeric() == true && iterEqResult->getConstant().isZero() == true) {
			iterParsed = preparsed.erase(iterParsed) - 1;
			iterEqResult = eqResult.erase(iterEqResult) - 1;
		}
	}

	if (eqResult.size() < 2) {
		return(false);
	}

	/*  Finalize  */
	eqResult.at(0).vectorAllToInteger(eqResult);
	eqResult.at(0).vectorRemoveOnlyUnknown(eqResult);

	/*  Arrange elements  */
	balancerArrangeSide(preparsed, eqResult);

	parsed = preparsed;
	result = eqResult;

	return(true);
}

/*
 *	bool balancerBuildAnswer(vector<cdec> &parsed, vector<polynomial> &result, string &built)
 *
 *	Mix the parsed chemical equation with balanced results and return the full result.
 */
bool balancerBuildAnswer(vector<cdec> &parsed, vector<polynomial> &result, string &built) {
	bool lastSide = false, requireEqual = false;
	string r, connector, tmpR, tmpSymbol;
	vector<cdec>::iterator iterParsed;
	vector<polynomial>::iterator iterEqResult;

	/*  Initialize  */
	r.clear();

	for (iterParsed = parsed.begin(), iterEqResult = result.begin(); iterParsed != parsed.end(); iterParsed++, iterEqResult++) {
		/*
		 *  Convert the result to string
		 *
		 *  P.S: If the prefix-number is 1, we can ignore it, e.g:
		 *       > [1]C+[1]O2=[1]CO2
		 *            C+   O2=   CO2
		 */
		if (iterEqResult->isNoUnknown() == true && iterEqResult->getConstant() == fraction(1, 1)) {
			tmpR.clear();
		} else {
			tmpR = iterEqResult->toString();
		}

		/*  Decide the connector  */
		if (iterParsed == parsed.begin()) {
			if (lastSide != iterParsed->right) {
				requireEqual = true;
			}
			if (iterParsed->negative == true) {
				connector = PARSER_SYNTAX_MINUS;
			} else {
				connector.clear();
			}
		} else {
			if (lastSide != iterParsed->right) {
				connector = PARSER_SYNTAX_EQUAL;
				lastSide = iterParsed->right;
			} else {
				connector = (iterParsed->negative == true ? PARSER_SYNTAX_MINUS : PARSER_SYNTAX_PLUS);
			}
		}

		/*  Add bracket for hydrate molecule formula  */
		if (findHydrateDot(iterParsed->symbol, NULL) == true && iterEqResult->getConstant() != fraction(1, 1)) {
			tmpSymbol = POLYNOMIAL_BRACKET_BEGIN + iterParsed->symbol + POLYNOMIAL_BRACKET_END;
		} else {
			tmpSymbol = iterParsed->symbol;
		}

		/*  Rebuild  */
		if (iterEqResult->isNoUnknown() == true) {
			r += connector + tmpR + tmpSymbol;
		} else {
			r += connector + POLYNOMIAL_BRACKET_BEGIN + tmpR + POLYNOMIAL_BRACKET_END + tmpSymbol;
		}
	}

	/*  If no "=" exists, exit  */
	if (lastSide == false || requireEqual == true) {
		return(false);
	}

	built = r;

	return(true);
}

/*
 *	bool balancerAuto(string &formula, string &result)
 *
 *	Balance a chemical equation automatically.
 */
bool balancerAuto(string &formula, string &result) {
	vector<cdec> parsed;
	vector<polynomial> eresult;

	if (balancerProcess(formula, parsed, eresult) == false) {
		result.clear();
		return(false);
	}

	if (balancerBuildAnswer(parsed, eresult, result) == false) {
		result.clear();
		return(false);
	}

	return(true);
}

