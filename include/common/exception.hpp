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

#ifndef __EXCEPTION_HPP__
	#define __EXCEPTION_HPP__

	#include <string>

	using namespace std;

	class xsvException {
	public:
		/*  Public declares  */
		xsvException();
		xsvException(const xsvException &src);
		xsvException(const int src_errno, const string &src_file, const int &src_line, const string &src_function, const string &src_description);
		~xsvException();
		int getID();
		int getLine();
		string getFile();
		string getFunction();
		string getDescription();
	private:
		/*  Private variables  */
		string file, function, description;
		int id, line;
	};

	/*  External  */
	#define ERRNO_SUCCESS         0
	#define ERROR_DIVIDE_ZERO     1
	#define ERROR_UNREACHABLE     2
	#define ERROR_OVERFLOW        3
	#define ERROR_MEMORY          4
#endif

