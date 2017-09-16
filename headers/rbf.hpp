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
#ifndef _BETACORE_RBF_H_
#define _BETACORE_RBF_H_
#include <cmath>
namespace betacore{
	struct RBF_Exception : public std::exception {
	const char * what () const throw () {
		return "RBF System Exception";
	}
	};
	template <typename T>
	struct Point {
		T x;
		T y;
		T z;
		explicit Point(T x = 0.0, T y=0.0, T z=0.0) : x(x), y(y), z(z) {}
	};
	template <typename T>
	class RBF{
		private:
		T Polyharmonic( T r, T n){
			if(r % 2 == 0){
				return (T) pow(r,2*n) *	log(r);
			}
			return (T) pow(r,2*n);
		}

		T Multiquadric(T r, T c){
			return (T) sqrt(r*r + c*c);
		}

		T Gaussian(T r, T c){
			return (T) exp(-1*c * r *r);
		}
		public:
		static const int POLYHARMONIC = 0;
		static const int MULTIQUADRIC = 1;
		static const int GAUSSIAN = 2;
		template<size_t rows> 
		RBF(Point (&p)[row], a,b,c,d, int type){
			T r;
			T theta;
			T A[row*row][row*row];
			for(int i=0; i < row -1; i++){
				T x_i = p[i].x;
				T y_i = p[i].y;
				T z_i = p[i].z;
				// j = i+1;
				T x_j = p[i+1].x;
				T y_j = p[i+1].y;
				T z_j = p[i+1].z;
				r = abs(x_j - x_i);
				
				T n = (T) row; // ..not sure about this
				switch(type){
					case POLYHARMONIC:
						theta = Polyharmonic (r, n);
						break;
					case MULTIQUADRIC:
						theta = Multiquadric (r,c);
						break;
					case GAUSSIAN:
						theta = Gaussian(r, c);
						break;
				}
			}
		}
		~RBF(){
		}
	};
}
#endif