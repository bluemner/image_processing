/** MIT License

	Copyright (c) 2017 Brandon Bluemner

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
 */
#ifndef _BETACORE_LAGRANGIAN_H_
#define _BETACORE_LAGRANGIAN_H_

namespace betacore {

	template <typename T>
	class Lagrangian{
		template <size_t size>
		static T Polynomial( T x, size_t point, T (&points)[size] ){
			size_t i;
			T h=1;
			for(i=0; i<size; i++){
				if(i != point){
					h = h * (x - points[i]) / (points[point] - points[i]);
				}
				return h;
			}
		}
		template <size_t size>
		static T Interpolant( T x, T (&points)[size], T (&values)[size] ){
			size_t i;
			T sum = (T) 0; // cast needed
			for( i = 0; i < size; i++){
				sum = sum + values[i] * Polynomial(x, i, points);
			}
			return sum;
		}
	}; 
}
#endif