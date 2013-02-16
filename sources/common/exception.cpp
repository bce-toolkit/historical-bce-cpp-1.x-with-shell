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
#include <common/exception.hpp>

using std::string;

/*
 *	xsvException::xsvException()
 *
 *	Constructor.
 */
xsvException::xsvException() {
	file = "";
	line = 0;
	function = "";
	description = "";
	id = ERRNO_SUCCESS;
}

/*
 *	xsvException::xsvException(const xsvException &src)
 *
 *	Copy constructor.
 */
xsvException::xsvException(const xsvException &src) {
	file = src.file;
	line = src.line;
	function = src.function;
	description = src.description;
	id = src.id;
}

/*
 *	xsvException::xsvException(const int src_id, const string &src_file, const int &src_line, const string &src_function, const string &src_description)
 *
 *	Construct from specified error.
 */
xsvException::xsvException(const int src_id, const string &src_file, const int &src_line, const string &src_function, const string &src_description) {
	file = src_file;
	line = src_line;
	function = src_function;
	description = src_description;
	id = src_id;
}

/*
 *	xsvException::~xsvException()
 *
 *	Destructor.
 */
xsvException::~xsvException() {
	/*  Dummy destructor  */
}

/*
 *	int xsvException::getID()
 *
 *	Get ID of the error.
 */
int xsvException::getID() {
	return(id);
}

/*
 *	int xsvException::getLine()
 *
 *	Get the line number where the error occured.
 */
int xsvException::getLine() {
	return(line);
}

/*
 *	string xsvException::getFile()
 *
 *	Get the file which threw the error.
 */
string xsvException::getFile() {
	return(file);
}

/*
 *	string xsvException::getFunction()
 *
 *	Get the function which threw the error.
 */
string xsvException::getFunction() {
	return(function);
}

/*
 *	string xsvException::getDescription()
 *
 *	Get description of the error.
 */
string xsvException::getDescription() {
	return(description);
}

