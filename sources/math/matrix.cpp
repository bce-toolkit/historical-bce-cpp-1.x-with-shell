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
#include <math/fraction.hpp>
#include <math/matrix.hpp>

using std::cout;
using std::endl;

/*
 *	matrix::matrix()
 *
 *	Constructor.
 */
matrix::matrix() {
	mtx = NULL;
	resizeMatrix(1, 1);
}

/*
 *	matrix::matrix(const matrix &src)
 *
 *	Copy constructor.
 */
matrix::matrix(const matrix &src) {
	size_t px, py;

	mtx = NULL;
	resizeMatrix(src.max_x, src.max_y);

	for (py = 0; py < max_y; py++) {
		for (px = 0; px < max_x; px++) {
			mtx[py][px] = src.mtx[py][px];
		}
	}
}

/*
 *	matrix::matrix(const size_t mx, const size_t my)
 *
 *	Construct with specified size.
 */
matrix::matrix(const size_t mx, const size_t my) {
	mtx = NULL;
	resizeMatrix(mx, my);
}

/*
 *	matrix::~matrix()
 *
 *	Destructor.
 */
matrix::~matrix() {
	destroyMatrix();
}

/*
 *	void matrix::clear()
 *
 *	Set all members of the matrix to zero.
 */
void matrix::clear() {
	size_t px, py;

	for (py = 0; py < max_y; py++) {
		for (px = 0; px < max_x; px++) {
			mtx[py][px].setValue(0, 1);
		}
	}
}

/*
 *	void matrix::resizeMatrix(const size_t mx, const size_t my)
 *
 *	Resize the matrix.
 */
void matrix::resizeMatrix(const size_t mx, const size_t my) {
	size_t py;

	if (mtx != NULL) {
		destroyMatrix();
	}

	max_x = mx;
	max_y = my;

	do {
		mtx = new fraction*[my];
	} while(mtx == NULL);

	for (py = 0; py < max_y; py++) {
		do {
			mtx[py] = new fraction[mx]();
		} while(mtx[py] == NULL);
	}
}

/*
 *	void matrix::destroyMatrix()
 *
 *	Destroy(destruct) the matrix.
 */
void matrix::destroyMatrix() {
	size_t py;

	for (py = 0; py < max_y; py++) {
		delete [](mtx[py]);
	}

	delete []mtx;
}

/*
 *	const fraction& matrix::read(const size_t ofx, const size_t ofy)
 *
 *	Read data from specified position.
 */
fraction& matrix::read(const size_t ofx, const size_t ofy) {
	return(mtx[ofy][ofx]);
}

/*
 *	void matrix::write(const size_t ofx, const size_t ofy, const fraction &data)
 *
 *	Write data to specified position.
 */
void matrix::write(const size_t ofx, const size_t ofy, const fraction &data) {
	mtx[ofy][ofx] = data;
}

/*
 *	void matrix::swapRow(size_t src, size_t dest)
 *
 *	Swap row @src and @dest.
 */
void matrix::swapRow(const size_t src, const size_t dest) {
	fraction *t;
	t = mtx[src];
	mtx[src] = mtx[dest];
	mtx[dest] = t;
}

/*
 *	void matrix::swapColumn(size_t src, size_t dest)
 *
 *	Swap column @src and @dest.
 */
void matrix::swapColumn(const size_t src, const size_t dest) {
	size_t py;
	fraction t;

	for (py = 0; py < max_y; py++) {
		t = mtx[py][src];
		mtx[py][src] = mtx[py][dest];
		mtx[py][dest] = t;
	}
}

/*
 *	void matrix::print()
 *
 *	Print the matrix out.
 */
void matrix::print() {
	size_t px, py;

	for (py = 0; py < max_y; py++) {
		for (px = 0; px < max_x; px++) {
			cout<<mtx[py][px].toString()<<'\t';
		}
		cout<<endl;
	}
}

/*
 *	matrix& matrix::operator=(const matrix &src)
 *
 *	Overload operator `[ptr: this] = @src`
 */
matrix& matrix::operator=(const matrix &src) {
	size_t px, py;

	if (this == &src) {
		return(*this);
	}

	resizeMatrix(src.max_x, src.max_y);

	for (py = 0; py < max_y; py++) {
		for (px = 0; px < max_x; px++) {
			mtx[py][px] = src.mtx[py][px];
		}
	}

	return(*this);
}

