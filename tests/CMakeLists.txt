#
#	Copyright (C) 2012-2013, The BCE Project authors. All rights reserved.
#
#	Redistribution and use in source and binary forms, with or without
#	modification, are permitted provided that the following conditions
#	are met:
#	1. Redistributions of source code must retain the above copyright
#	   notice, this list of conditions and the following disclaimer.
#	2. Redistributions in binary form must reproduce the above copyright
#	   notice, this list of conditions and the following disclaimer in the
#	   documentation and/or other materials provided with the distribution.
#
#	THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
#	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#	ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
#	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
#	OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
#	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
#	OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#	SUCH DAMAGE.
#

#  Source codes
IF (MINGW)
	SET(BCDPLUS_TESTS_SRCS
		"tests_runner.cpp"
		"balancer/balancer_tests.cpp"
		"math/integer/integer_tests.cpp"
		"math/fraction/fraction_tests.cpp"
		"math/equation/equation_tests.cpp"
		"parser/molecule/molecule_tests.cpp"
		"parser/preparser/preparser_tests.cpp"
		"resources/version.rc"
	)
ELSE (MINGW)
	SET(BCDPLUS_TESTS_SRCS
		"tests_runner.cpp"
		"balancer/balancer_tests.cpp"
		"math/integer/integer_tests.cpp"
		"math/fraction/fraction_tests.cpp"
		"math/equation/equation_tests.cpp"
		"parser/molecule/molecule_tests.cpp"
		"parser/preparser/preparser_tests.cpp"
	)
ENDIF (MINGW)

ADD_EXECUTABLE(bceplus_tests ${BCDPLUS_TESTS_SRCS})
SET_TARGET_PROPERTIES(bceplus_tests PROPERTIES OUTPUT_NAME "bce++_tests")
TARGET_LINK_LIBRARIES(bceplus_tests bceplus)
