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
#include <cstdio>
#include <math/integer.hpp>
#include "math/integer/integer_tests.hpp"
#include "math/fraction/fraction_tests.hpp"
#include "math/equation/equation_tests.hpp"
#include "parser/molecule/molecule_tests.hpp"
#include "parser/preparser/preparser_tests.hpp"
#include "balancer/balancer_tests.hpp"
#include "tests_runner.hpp"

using namespace std;

struct t_unit_test_table tests[] = {
	{
		.name = "math/integer.cpp [Basic test]",
		.path = "math/integer/basic/integer",
		.lower = 1,
		.higher = 5,
		.test = integer_unit_test
	},
	{
		.name = "math/integer.cpp [Extra test]",
		.path = "math/integer/extra/integer",
		.lower = 1,
		.higher = 100,
		.test = integer_unit_test
	},
	{
		.name = "math/integer.cpp [gcd() test]",
		.path = "math/integer/gcd/integer",
		.lower = 1,
		.higher = 10,
		.test = gcd_unit_test
	},
	{
		.name = "math/integer.cpp [lcm() test]",
		.path = "math/integer/lcm/integer",
		.lower = 1,
		.higher = 10,
		.test = lcm_unit_test
	},
	{
		.name = "math/equation.cpp",
		.path = "math/equation/equation",
		.lower = 1,
		.higher = 434,
		.test = equation_unit_test
	},
	{
		.name = "math/fraction.cpp",
		.path = "math/fraction/fraction",
		.lower = 1,
		.higher = 100,
		.test = fraction_unit_test
	},
	{
		.name = "parser/molecule.cpp",
		.path = "parser/molecule/molecule",
		.lower = 1,
		.higher = 461,
		.test = molecule_unit_test
	},
	{
		.name = "parser/molecule.cpp",
		.path = "bugs/parser/molecule/molecule",
		.lower = 1,
		.higher = 1,
		.test = molecule_unit_test
	},
	{
		.name = "parser/syntax.cpp [Preparser test]",
		.path = "parser/preparser/preparser",
		.lower = 1,
		.higher = 434,
		.test = preparser_unit_test
	},
	{
		.name = "balancer.cpp",
		.path = "balancer/balancer",
		.lower = 1,
		.higher = 434,
		.test = balancer_unit_test
	},
	{
		.name = "balancer.cpp",
		.path = "bugs/balancer/balancer",
		.lower = 1,
		.higher = 4,
		.test = balancer_unit_test
	},
	{
		.name = "(None)",
		.path = "",
		.lower = 1,
		.higher = 1,
		.test = NULL
	},
};

/*
 *	bool tester_check_result(const string &path, const string &sample)
 *
 *	Check whether the answer file equals to our sample file.
 */
bool tester_check_result(const string &path, const string &sample) {
	ifstream fa, fb;
	string ia, ib;

	fa.open(path.c_str(), ifstream::in);
	fb.open(sample.c_str(), ifstream::in);

	while(!fa.eof() && !fb.eof()) {
		fa >> ia;
		fb >> ib;
		if (ia != ib) {
			fa.close();
			fb.close();
			return(false);
		}
	}
	
	if (fa.eof() == false || fb.eof() == false) {
		return(false);
	}
	
	fa.close();
	fb.close();
	
	return(true);
}

/*
 *	int main(int argc, char **argv)
 *
 *	Main processor.
 */
int main(int argc, char **argv) {
	string pathA, pathB;
	struct t_unit_test_table *ptrTest;
	int idx;
	const string tmpFile = "answer.tmp";

	for (ptrTest = tests; ptrTest->test != NULL; ptrTest++) {
		for (idx = ptrTest->lower; idx <= ptrTest->higher; idx++) {
			pathA = ptrTest->path + integer(idx).toString() + ".in";
			pathB = ptrTest->path + integer(idx).toString() + ".out";
			cout << "[" << ptrTest->name << ": " << pathA << "]" << endl;
			ptrTest->test(pathA, tmpFile);
			if (tester_check_result(pathB, tmpFile) == false) {
				cout << "[ERROR] Failed: " << pathA << endl;
				return(1);
			}
		}
	}

	if (remove(tmpFile.c_str()) != 0) {
		cout << "[ERROR] Cannot remove '" << tmpFile << "'." << endl;
	}

	cout << "[SUCCEED] Passed all test cases." << endl;

	return(0);
}

