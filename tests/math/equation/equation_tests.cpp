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

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <math/fraction.hpp>
#include <math/matrix.hpp>
#include <math/polynomial.hpp>
#include <math/equation.hpp>
#include "equation_tests.hpp"

using namespace std;

/*
 *	void equation_unit_test(string inpath, string outpath)
 *
 *	Unit test.
 */
void equation_unit_test(string inpath, string outpath) {
	size_t mx, my, px, py;
	int member;
	ifstream infile;
	ofstream offile;
	matrix mtx;
	polynomial tmp;
	vector<polynomial> result;
	vector<polynomial>::iterator iterResult;

	infile.open(inpath.c_str(), ifstream::in);
	offile.open(outpath.c_str(), ofstream::out);

	infile >> mx >> my;

	mtx.resizeMatrix(mx, my);

	for (py = 0; py < my; py++) {
		for (px = 0; px < mx; px++) {
			infile >> member;
			mtx.write(px, py, fraction(member, 1));
		}
	}

	result.clear();
	if (equationSolve(mtx, 0, 0, mx, my, result) == false) {
		infile.close();
		offile.close();
		return;
	}

	tmp.vectorReindex(result);

	for (iterResult = result.begin(); iterResult != result.end(); iterResult++) {
		offile << iterResult->toString() << endl;
	}

	infile.close();
	offile.close();
}

