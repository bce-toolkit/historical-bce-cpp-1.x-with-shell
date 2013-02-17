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

#include <vector>
#include <math/integer.hpp>
#include <math/fraction.hpp>
#include <math/polynomial.hpp>
#include <math/matrix.hpp>
#include <math/equation.hpp>

using namespace std;

/*
 *	bool equationSolve(matrix &mtx, size_t ofx, v ofy, size_t mx, size_t my, vector<polynomial> &result)
 *
 *	Solve linear equations  by using Gaussian-elimination.
 *	(Matrix: @mtx, X-offset: @ofx, Y-offset: @ofy, size: @mx * @my, result: @result)
 */
bool equationSolve(matrix &mtx, size_t ofx, size_t ofy, size_t mx, size_t my, vector<polynomial> &result) {
	size_t px, py;
	bool exchanged = false;
	vector<polynomial> rsvResult;
	polynomial build, tmp;

	/*  Check matrix size  */
	if (mx < 2 || my == 0) {
		return(false);
	}

	/*
	 *  Reroute to make the top-left element of the matrix a non-zero value, eg:
	 *  > 0 1 3   =>  > 1 2 2
	 *  > 1 2 2       > 0 1 3
	 *    1 0 -4        1 0 -4
	 */
	for (py = 0; py < my; py++) {
		if (mtx.read(ofx, ofy + py).isZero() == false) {
			mtx.swapRow(ofy, ofy + py);
			exchanged = true;
			break;
		}
	}

	if (exchanged == false) {
		/*  If elements in the first column are all zero, remove the first column and continue solving recursively.  */
		rsvResult.clear();

		/*  Continue solving the matrix without the first column  */
		if (mx > 2 && equationSolve(mtx, ofx + 1, ofy, mx - 1, my, rsvResult) == false) {
			return(false);
		}

		/*  Add the unknown corresponding to the first column of the matrix to the unknown table of the result  */
		build.clear();
		build.setConstant(fraction(0, 1));
		build.addUnknown(integer(ofx), fraction(1, 1));
		rsvResult.insert(rsvResult.begin(), build);
		result = rsvResult;

		return(true);
	}

	if (mx == 2 || my == 1) {
		/*  Recursive endpoint: to get the value of remain unknowns  */
		rsvResult.clear();
		build.clear();
		build.setConstant(mtx.read(ofx + mx - 1, ofy));

		for (px = 1; px <= mx - 2; px++) {
			tmp.clear();
			tmp.setConstant(fraction(0, 1));
			tmp.addUnknown(integer(ofx) + integer(px), fraction(1, 1));
			rsvResult.insert(rsvResult.end(), tmp);
			build.addUnknown(integer(ofx) + integer(px), fraction(0, 1) - mtx.read(ofx + px, ofy));
		}

		build.multiply(fraction(1, 1) / mtx.read(ofx, ofy));
		rsvResult.insert(rsvResult.begin(), build);
		result = rsvResult;
	} else {
		/*  Eliminate  */
		for (py = 0; py < my; py++) {
			if (mtx.read(ofx, ofy + py).isZero() == false) {
				for (px = 0; px < mx; px++) {
					if (py == 0) {
						mtx.write(ofx + mx - px - 1, ofy + py, mtx.read(ofx + mx - px - 1, ofy + py) / mtx.read(ofx, ofy + py));
					} else {
						mtx.write(ofx + mx - px - 1, ofy + py, mtx.read(ofx + mx - px - 1, ofy) - mtx.read(ofx + mx - px - 1, ofy + py) / mtx.read(ofx, ofy + py));
					}
				}
			}
		}

		/*  Calculate the value of eliminated equations  */
		rsvResult.clear();
		if (equationSolve(mtx, ofx + 1, ofy + 1, mx - 1, my - 1, rsvResult) == false) {
			return(false);
		}

		/*  Back substitution  */
		build.clear();
		build.setConstant(mtx.read(ofx + mx - 1, ofy));

		for (px = 1; px < mx - 1; px++) {
			tmp = rsvResult.at(px - 1);
			tmp.multiply(mtx.read(ofx + px, ofy));
			build.mergeCost(tmp);
		}

		rsvResult.insert(rsvResult.begin(), build);
		result = rsvResult;
	}

	return(true);
}

/*
 *	bool equationCheckResult(const matrix &mtx, size_t mx, size_t my, vector<polynomial> &result)
 *
 *	Check the result returned by equationSolve().
 */
bool equationCheckResult(const matrix &mtx, size_t mx, size_t my, vector<polynomial> &result) {
	size_t px, py;
	polynomial checker, tmp;

	for (py = 0; py < my; py++) {
		/*  Check each equation  */
		checker.clear();
		checker.setConstant(fraction(0, 1));

		for (px = 0; px < mx - 1; px++) {
			tmp = result.at(px);
			tmp.multiply((const_cast<matrix&>(mtx)).read(px, py));
			checker.merge(tmp);
		}

		if (checker.isNumeric() == false || checker.getConstant() != (const_cast<matrix&>(mtx)).read(mx - 1, py)) {
			return(false);
		}
	}

	return(true);
}

