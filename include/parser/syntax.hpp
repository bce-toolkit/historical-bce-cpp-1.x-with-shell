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

#ifndef __SYNTAX_HPP__
	#define __SYNTAX_HPP__

	#include <string>
	#include <vector>
	#include <math/matrix.hpp>

	using namespace std;

	struct t_syntax_chemical_equation_decoder {
		string symbol;
		bool negative, right;
	};
	typedef struct t_syntax_chemical_equation_decoder cdec;

	/*  Declares  */
	vector<cdec> syntaxPreparser(const string formula);
	bool syntaxDecodedToMatrix(vector<cdec> &cdecTable, matrix &target, size_t &mx, size_t &my);

	/*  Defination  */
	#define PARSER_SYNTAX_PLUS "+"
	#define PARSER_SYNTAX_MINUS "-"
	#define PARSER_SYNTAX_SEPARATOR ";"
	#define PARSER_SYNTAX_EQUAL "="
#endif

