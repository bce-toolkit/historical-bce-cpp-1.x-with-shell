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

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bce-gui
TEMPLATE = app

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

SOURCES += main.cpp\
        bce.cpp \
    ../../sources/common/exception.cpp \
    ../../sources/math/polynomial.cpp \
    ../../sources/math/polynomial_unknown.cpp \
    ../../sources/math/matrix.cpp \
    ../../sources/math/integer.cpp \
    ../../sources/math/fraction.cpp \
    ../../sources/math/equation.cpp \
    ../../sources/parser/syntax.cpp \
    ../../sources/parser/molecule.cpp \
    ../../sources/parser/element.cpp \
    ../../sources/parser/common.cpp \
    ../../sources/balancer.cpp

HEADERS  += bce.hpp \
    ../../include/common/exception.hpp \
    ../../include/math/polynomial.hpp \
    ../../include/math/polynomial_unknown.hpp \
    ../../include/math/matrix.hpp \
    ../../include/math/integer.hpp \
    ../../include/math/fraction.hpp \
    ../../include/math/equation.hpp \
    ../../include/parser/syntax.hpp \
    ../../include/parser/molecule.hpp \
    ../../include/parser/element.hpp \
    ../../include/parser/common.hpp \
    ../../include/parser/bracket.hpp \
    ../../include/balancer.hpp \
    ../../version.h \
    setup.hpp \
    variable.hpp

FORMS    += bce.ui

OTHER_FILES += \
    version.rc \
    bce++.ts

TRANSLATIONS = nls/en-US/bce++.ts \
               nls/ja-JP/bce++.ts \
               nls/ko-KR/bce++.ts \
               nls/posix/bce++.ts \
               nls/zh-CN/bce++.ts \
               nls/zh-TW/bce++.ts

win32: RC_FILE += resources/version.rc

