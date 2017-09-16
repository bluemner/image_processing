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
#ifndef _BETACORE_BI_LINEAR_INTERPOLATION_H_
#define _BETACORE_BI_LINEAR_INTERPOLATION_H_


#ifndef _BETACORE_LINEAR_SYSTEM_H_
#include "./linear_system.hpp"
#endif

namespace betacore
{
template <typename T>
struct point_value
{
	T x;
	T y;
	T value;
};
template <typename T>
class Bi_Linear
{
  private:
	Linear_System<T> * linear_system;
	point_value<T> result;
	void make_linear_system(const T &x,
							const T &y,
							const point_value<T> &I_00, 
							const point_value<T> &I_01,
							const point_value<T> &I_10,
							const point_value<T> &I_11)
	{
		const size_t n = 4;
	
		T A [n][n];
	
	
		T  b[n];
		T  x_vector[n];
		
		//Column 0 set up
		A[0][0] = 1;
		A[1][0] = 1;
		A[2][0] = 1;
		A[3][0] = 1;
		
		//Column 1 set up
		A[0][1] = I_00.x;
		A[1][1] = I_01.x;
		A[2][1] = I_10.x;
		A[3][1] = I_11.x;

		//Column 2 set up
		A[0][2] = I_00.y;
		A[1][2] = I_01.y;
		A[2][2] = I_10.y;
		A[3][2] = I_11.y;

		
		//Column 3 set up
		A[0][2] = I_00.y * I_00.x;
		A[1][2] = I_01.y * I_01.x;
		A[2][2] = I_10.y * I_10.x;
		A[3][2] = I_11.y * I_11.x;
	
		linear_system = new Linear_System(A,x_vector,b);
		linear_system.get_x(x_vector);
		T a = x_vector[0];
		T b = x_vector[1];
		T c = x_vector[2];
		T d = x_vector[3];
		result.value = (T) (a*x+b*y+c*x*y+d);
	}
  public:
	// Bi_Linear(T &x_0, T &x_1, T &y_0, T &y_1,
	// 		  T &I_00, T &I_01, T &I_10, T &I_11)
	// {
	// 	throw "No implemented";
	// }
	Bi_Linear(){
		this->linear_system= nullptr;
	}
	/**
		Args: 
			point_value<T> &I_00 point stucture for pont at grid 00 
			I_01 ---- I_11
			|	I_xy	|
			I_00------I_10
	*/
	Bi_Linear(const T &x,
			  const T &y,
			  const point_value<T> &I_00,
			  const point_value<T> &I_01,
			  const point_value<T> &I_10,
			  const point_value<T> &I_11)
	{

		this->linear_system= nullptr;
		this->result.x=x;
		this->result.y=y;
		this->result.value = (T) 0;
	}
	~Bi_Linear(){
		if(linear_system != nullptr){
			delete this->linear_system;
			this->linear_system= nullptr;
		}
	}
	T get_value(){
		return this->result.value;
	}
};
}

#endif