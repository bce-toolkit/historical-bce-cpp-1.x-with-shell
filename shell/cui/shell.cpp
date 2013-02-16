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
#include <algorithm>
#include <cctype>
#include <string>
#include <iterator>
#include <balancer.hpp>
#include "shell.hpp"

/*  For compatible of isatty()  */
#include <cstdio>
#include <unistd.h>

using namespace std;

/*
 *	void removeSpaces(string &dest)
 *
 *	Remove all spaces in a string.
 */
void removeSpaces(string &dest) {
	string r = "";
	string::iterator it;
	for (it = dest.begin(); it != dest.end(); it++) {
		if (*it == PROGRAM_CHAR_DESCRIPTOR) {
			break;
		}
		if (!isspace(*it)) {
			r.insert(r.end(), *it);
		}
	}
	dest = r;
}

/*
 *	int main(int argc, char **argv)
 *
 *	Main processor.
 */
int main(int argc, char **argv) {
	string line, result;
	int idx;

	if (argc != 1) {
		for (idx = 1; idx < argc; idx++) {
			line = argv[idx];
			removeSpaces(line);

			if (line.length() == 0) {
				continue;
			}

			if (balancerAuto(line, result) == false) {
				cout << PROGRAM_STRING_NOBC << endl;
			} else {
				cout << result << endl;
			}
		}
		return(PROGRAM_ERROR_SUCCESS);
	}

	while(cin.eof() == false) {
		if (isatty(fileno(stdin))) {
			cout << PROGRAM_STRING_PROMPT;
		}

		getline(cin, line);
		removeSpaces(line);

		if (line.length() == 0) {
			continue;
		}

		if (line == PROGRAM_STRING_EXIT) {
			break;
		}

		if (balancerAuto(line, result) == false) {
			cout << PROGRAM_STRING_NOBC << endl;
		} else {
			cout << result << endl;
		}
	}

	return(PROGRAM_ERROR_SUCCESS);
}

