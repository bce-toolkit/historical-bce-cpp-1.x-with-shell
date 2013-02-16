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

#ifndef __MOLECULE_HPP__
	#define __MOLECULE_HPP__

	#include <string>
	#include <vector>
	#include <parser/element.hpp>

	using std::string;
	using std::vector;

	/*  Definations  */
	#define PARSER_MOLECULE_BRACKET_BEGIN "("
	#define PARSER_MOLECULE_BRACKET_END ")"
	#define PARSER_MOLECULE_NUMBER_ONE "1"

	/*  Declares  */
	bool parseMolecule(const string &formula, int *status, const integer &suffix, vector<element> &result);
#endif

#ifdef __REQUIRE_MOLECULE_REPLACE_TABLE__
	#ifndef __DECLARED_MOLECULE_REPLACE_TABLE__
		#define __DECLARED_MOLECULE_REPLACE_TABLE__

		#include <string>
		#include <math/integer.hpp>

		using std::string;

		/*  Status of chemical  */
		#define STATUS_NONE -1
		#define STATUS_GAS 0
		#define STATUS_LIQUID 1
		#define STATUS_SOLID 2
		#define STATUS_AQUEOUS 3

		/*  Table ADT  */
		struct replaceTable {
			string name;
			string target;
			integer suffix;
			int status;
		};

		/*  Replace table  */
		struct replaceTable replace_table[] = {
			{.name = "l",	.target = "",	.suffix = integer(1),	.status = STATUS_LIQUID},
			{.name = "g",	.target = "",	.suffix = integer(1),	.status = STATUS_GAS},
			{.name = "s",	.target = "",	.suffix = integer(1),	.status = STATUS_SOLID},
			{.name = "aq",	.target = "",	.suffix = integer(1),	.status = STATUS_AQUEOUS},
			{.name = "e+",	.target = "e",	.suffix = integer(1),	.status = STATUS_NONE},
			{.name = "e-",	.target = "e",	.suffix = integer(-1),	.status = STATUS_NONE},
			{.name = "",	.target = "",	.suffix = integer(0),	.status = STATUS_NONE}
		};
	#endif
#endif

