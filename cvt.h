/*
 * MIT License
 *
 * Copyright 2016 Marios Sofokleous
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 * 
 * File: cvt.h
 * Description: Calculation of the generator points for the approximate CVT
 * Date: 09.05.2016
 * Author: Sofokleous Marios
*/

#ifndef CVT_H
#define CVT_H

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>
#include <cfloat>
using namespace std;

// Row of the matrix
typedef vector<double> Row;
// Matrix
typedef vector<Row> Matrix;

// Point class
class point{
	private:
		vector<double> terms;
		const int dim;
	public:
		// Ctor
		point(int dim);
		// Methods
		void print();
		void rand_init(const Matrix& bounds);
		double sqr_dist(const point& q) const;
		double get_term(int dim) const;
		void set_term(int dim, double val);
};

// Functions
double random_float(double a, double b);

/*
 * Inputs:
 *   k: # of generators
 *	 q: # of sample points
 *   dim: dimensionality
 *   epochs: # of iterations
 *   bounds: search-space boundaries
 *   a1, a2, b1, b2: parameters, where: a2>0, b2>0, a1+a2=1, b1+b2=1
 *
 * Output:
 *   z generator points for the approximate CVT
 */
vector<point> CVT(
	int k, 
	int q, 
	int dim, 
	int epochs, 
	const Matrix& bounds, 
	double a1, 
	double a2, 
	double b1,
	double b2
);

#endif