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

#  Check for CMake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#  Load RC compiler
IF (MINGW)
	SET(CMAKE_RC_COMPILER_INIT "windres")
	ENABLE_LANGUAGE(RC)
	SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
ENDIF (MINGW)

#  Load projects
PROJECT("bce++")

#  Load include directories
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include")

#  Library sources
ADD_SUBDIRECTORY(sources)

#  Shell sources
IF (OFFICIAL_BUILD OR WITH_SHELL)
	ADD_SUBDIRECTORY(shell)
ENDIF (OFFICIAL_BUILD OR WITH_SHELL)

#  Unit tests sources
IF (OFFICIAL_BUILD OR WITH_UNIT_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF (OFFICIAL_BUILD OR WITH_UNIT_TESTS)
