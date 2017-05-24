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
 * File: main.cpp
 * Description: Calculation of the generator points for the approximate CVT
 * Date: 09.05.2016
 * Author: Sofokleous Marios
*/

#include <iostream>
#include <ctime>
#include "cvt.h"

// Row of the matrix
typedef vector<double> Row;
// Matrix
typedef vector<Row> Matrix;

int main(){

	// Feasible region(boundaries)
	Matrix bounds;
	// # of generators
	int k;
	// # of sample points
	int q;
	// Dimensionality
	int dim;
	// # of iterations
	int epochs;
	// Lower boundary 
	double lo;
	// Upper boundary
	double up;
	// Parameters
	double a1, a2, b1, b2;

	// User input
	cout << "# of generators: ";
	cin >> k;
	cout << "# of sampling points: ";
	cin >> q;
	cout << "Dimensionality: ";
	cin >> dim;
	cout << "# of iterations: ";
	cin >> epochs;
	cout << "a1 a2 b1 b2 (where: a2>0, b2>0, a1+a2=1, b1+b2=1): ";
	cin >> a1 >> a2 >> b1 >> b2;
	bounds.resize(dim);
	for(int i=0; i<dim; i++){
		cout << "Dimension#" << i+1 << "/lo: ";
		cin >> lo;
		bounds[i].push_back(lo);
		cout << "Dimension#" << i+1 << "/up: ";
		cin >> up;
   	bounds[i].push_back(up);
	}

	/*
	 * Inputs:
	 *   k: # of generators
	 *	 q: # of sampling points
	 *   dim: dimensionality
	 *   epochs: # of iterations
	 *   bounds: search-space boundaries
	 *   a1, a2, b1, b2: parameters, where: a2>0, b2>0, a1+a2=1, b1+b2=1
	 *
	 * Output:
	 *   z generator points for the approximate CVT
	 */
	vector<point> z = CVT(k, q, dim, epochs, bounds, a1, a2, b1, b2);

	// Print generator points 
	cout<<"\nGenerator points:\n\n";
	for(int i=0; i<z.size(); i++){
		z[i].print();
	}

	return 0;
}
