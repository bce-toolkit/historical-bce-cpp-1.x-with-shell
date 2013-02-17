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
#include <string>
#include <math/fraction.hpp>
#include "fraction_tests.hpp"

using namespace std;

/*
 *	void fraction_unit_test(string inpath, string outpath)
 *
 *	Unit test.
 */
void fraction_unit_test(string inpath, string outpath) {
	ifstream infile;
	ofstream offile;
	string input1, input2;
	fraction numa, numb;

	infile.open(inpath.c_str(), ifstream::in);
	offile.open(outpath.c_str(), ofstream::out);

	infile >> input1;
	infile >> input2;
	numa.setValue(input1, input2);

	infile >> input1;
	infile >> input2;
	numb.setValue(input1, input2);

	offile << (numa + numb).toString() <<endl;
	offile << (numa - numb).toString() <<endl;
	offile << (numa * numb).toString() <<endl;
	if (numb.isZero() == false) {
		offile << (numa / numb).toString() <<endl;
	} else {
		offile << "Divide by zero." <<endl;
	}
	offile << static_cast<int> (numa == numb) << endl;
	offile << static_cast<int> (numa != numb) << endl;
	offile << static_cast<int> (numa < numb) << endl;
	offile << static_cast<int> (numa <= numb) << endl;
	offile << static_cast<int> (numa > numb) << endl;
	offile << static_cast<int> (numa >= numb) << endl;

	infile.close();
	offile.close();
}

