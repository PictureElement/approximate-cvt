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
 * File: cvt.cpp
 * Description: Calculation of the generator points for the approximate CVT
 * Date: 09.05.2016
 * Author: Sofokleous Marios
*/

#include "cvt.h"

point::point(int dim) : dim(dim){
	terms.resize(dim);
	for(int i=0; i<dim; i++)
		terms[i] = 0.0;
}

void point::print(){
	for(int i=0; i<dim-1; i++)
		cout<<terms[i]<<" ";
	cout<<terms[dim-1]<<endl;
}

void point::rand_init(const Matrix& bounds){
	for(int i=0; i<dim; i++)
		terms[i] = random_float(bounds[i][0],bounds[i][1]);
}

double point::sqr_dist(const point& p) const{
	double squared_dist = 0.0;
	for(int i=0; i<dim; i++)
		squared_dist = squared_dist + 
									 (terms.at(i)-p.get_term(i)) * 
									 (terms.at(i)-p.get_term(i));
	return squared_dist;
}

double point::get_term(int dim) const{
	return terms.at(dim);
}

void point::set_term(int dim, double val){
	terms.at(dim) = val;
}

/*
 * Random number distribution that produces floating-point values according to a 
 * uniform distribution (every value is equally probable).
 */
double random_float(double a, double b)
{
	// Use a non-deterministic seed
	static random_device rd;
	// Mersenne_twister_engine is a random number engine
	static mt19937 gen(rd());
	// Produce a random number in the range [a,b)
	uniform_real_distribution<double> distr(a,b);
	return distr(gen);
}

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
){
	// Row of the matrix
 	typedef vector<point> Row;
 	// Matrix
 	typedef vector<Row> Matrix_2;

 	int iter = 0;

/*----------------------------------------------------------------------------*/

	// k generator points
	vector<point> set_z;
	// Indices, used to keep track of the # of updates on each generator
	int index[k];
	// Initialize the generators by using a Monte Carlo method
	for(int i=0; i<k; i++){
		set_z.push_back(point(dim));
		set_z[i].rand_init(bounds);
		index[i] = 1;
	}

/*----------------------------------------------------------------------------*/

	// q random points
	vector<point> set_y;
	for(int i=0; i<q; i++)
		set_y.push_back(point(dim));

	Matrix_2 set_w;
	set_w.resize(k);

/*----------------------------------------------------------------------------*/

	do{
		// GENERATE q RANDOM POINTS (SAMPLING POINTS)
		for(int i=0; i<q; i++)
			set_y[i].rand_init(bounds);

		// BUILD w (WHERE: w[i][] ARE SAMPLING POINTS CLOSEST TO z[i])
		for(int i=0; i<q; i++){
   		double squared_dist;
     	// Maximum finite representable floating-point number
    	double min_squared_dist = DBL_MAX;
     	int gen;
			for(int j=0; j<k; j++){
				squared_dist = set_y[i].sqr_dist(set_z[j]);
				if(squared_dist<min_squared_dist){
					min_squared_dist = squared_dist;
					gen = j;
				}
			}
			set_w[gen].push_back(set_y[i]);
			// Reset
			min_squared_dist = DBL_MAX;
		}

		// u[i] centroid of the set w[i][]
		vector<point> set_u;
		for(int i=0; i<k; i++)
			set_u.push_back(point(dim));

		// CALCULATE CENTROIDS
		// Select term
		for(int a=0; a<dim; a++){
   		double avg = 0.0;
			// Select set
			for(int b=0; b<k; b++){
				// Select point
				for(int c=0; c<set_w[b].size(); c++)
					avg = avg + set_w[b][c].get_term(a);
				avg = avg / set_w[b].size();
				set_u[b].set_term(a,avg);
			}
		}

		// UPDATE GENERATOR
		for(int i=0; i<k; i++){
			if(set_w[i].size() == 0)
				continue;
			else{
      	double value;
				for(int j=0; j<dim; j++){
					value = ((((a1*index[i])+b1) * set_z[i].get_term(j)) +
									 (((a2*index[i])+b2) * set_u[i].get_term(j))) / 
					           (index[i]+1);
					set_z[i].set_term(j,value);
				}
				// Update index associated with the updated generator
				index[i] = index[i] + 1;
			}
		}

		iter++;

	}while(iter<epochs);

	return set_z;
}