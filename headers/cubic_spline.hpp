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
#ifndef _BETACORE_CUBIC_SPLINE_H_
#define _BETACORE_CUBIC_SPLINE_H_

#ifndef _BETACORE_LINEAR_SYSTEM_H_
#include "./linear_system.hpp"
#endif
namespace betacore{
struct Cubic_Spline_Exception : public std::exception {
   const char * what () const throw () {
      return "Linear System Exception";
   }
};
	template <typename T>
	class Cubic_Spline{
		private:
			Linear_System<T> * linear_system;
			T left_bound_value;
			T right_bound_value;
			int left_bound_order;
			int right_bound_order;
			template<size_t rows, size_t cols>
			void make_linear_system(T (&data)[rows][cols], int mode){
				const size_t n = 4;

				T matrix[4][4];
				T right_hand_side[4];
				T L[n][n];
				T U[n][n]; 

				T  b[n];
				T  x[n];
				for(int i=0; i<n; i++){
					//b[i]= 3 * (data[i+1][2]-data[i][2]);
					x[i]= 1;// just to have a value;
				}
				//Only need to populate the upper matrix
				if(mode ==0){			
					T SL = left_bound_value;
					T SR = right_bound_value;
					for(int i=1; i < rows ;i++){
						T XL=data[i-1][0];
						T YL=data[i-1][1];
						T XR=data[i][0];
						T YR=data[i][1];
						
						matrix[0][0]=(T)1;
						matrix[0][1]=(T)0;
						matrix[0][2]=(T)0;
						matrix[0][3]=(T)0;

						matrix[1][0]=(T)0;
						matrix[1][1]=(T)1;
						matrix[1][2]=(T)0;
						matrix[1][3]=(T)0;

						matrix[2][0]=(T)1;
						matrix[2][1]=(T)(XR-XL);
						matrix[2][2]=(T)(XR-XL)*(XR-XL);
						matrix[2][3]=(T)0;

						matrix[3][0]=(T)0;
						matrix[3][1]=(T)1;
						matrix[3][2]=(T)2 *(XR-XL);
						matrix[3][3]=(T)(XR-XL)*(XR-XL);
						
						right_hand_side[0]= YL;
						right_hand_side[1]= SL;
						right_hand_side[2]= YR;
						right_hand_side[3]= SR;

						this->linear_system=new Linear_System<T>( matrix,x,right_hand_side);
						this->linear_system->forward_substitution();
						//this->linear_system->print();
						SR=SL;
						SL=this->linear_system->SL();
						
						for(double k=XL; k<XR; k+=0.1){
							//std::cout<<std::fixed<<std::setprecision(2)<< this->linear_system->value(k,XL,XR)<<std::endl ;
							std::cout<<std::fixed<<std::setprecision(2)<<"("<< k<< ","<<this->linear_system->value(k,XL,XR)<<")"<<std::endl ;
						}
					}
							
				}else if(mode ==1){
					for(int i=1; i<n+1; i++) {
						matrix[i][i-1] = (T)(1.0/3.0) * (data[i][1]-data[i-1][1]);
						matrix[i][i]   = (T)(2.0/3.0) * (data[i+1][1]-data[i-1][1]);
						matrix[i][i+1] = (T)(1.0/3.0) * (data[i+1][1]-data[i][1]);
						right_hand_side[i]=(data[i+1][2]-data[i][2])/(data[i+1][1]-data[i][1]) - (data[i][2]-data[i-1][2])/(data[i][1]-data[i-1][1]);
					}
					if(left_bound_order == FIRST_DERIVATIVE){
							matrix[n-1][n-1]=(T)2.0*(data[n-1][1]-data[n-2][1]);
							matrix[n-1][n-2]=(T)1.0*(data[n-1][1]-data[n-2][1]);
							right_hand_side[n-1]=3.0*(right_bound_value-(data[n-1][2]-data[n-2][2])/(data[n-1][1]-data[n-2][1]));
					}else if(left_bound_order == SECOND_DERIVATIVE){
						matrix[0][0] = 2.0;
						matrix[0][1] = 0.0;
						right_hand_side[0]=left_bound_value;
					}
				}else{
					//TODO;
				}

				// std::cout<<"Cubic spline matrix A:"<<std::endl;
				// Matrix<T>(matrix).print();
				// std::cout<<"LU:"<<std::endl;
				// std::cout<<"L:"<<std::endl;
				// Matrix<T>(L).print();
				// std::cout<<"U:"<<std::endl;
				// Matrix<T>(U).print();
				// std::cout<<"Loading U into linear_system Pre backward_substitution"<<std::endl;
				// this->linear_system=new Linear_System<T>( U,x,b);
				// this->linear_system->print();
			}
		public:
			
			static const int FIRST_DERIVATIVE=1;
			static const int SECOND_DERIVATIVE=2;
			
			Cubic_Spline(){	}
			~Cubic_Spline(){
				delete this->linear_system;
				this->linear_system= nullptr;
			}
			template<size_t rows, size_t cols>
			Cubic_Spline(T (&data)[rows][cols]){
			

				this->left_bound_value= (T) 0;
				this->left_bound_order= SECOND_DERIVATIVE;

				this->right_bound_value= (T) 0;
				this->right_bound_order= SECOND_DERIVATIVE;
				this->make_linear_system(data, 0);
		
				
			}
			void print(){
				std::cout<<"Loading U into linear_system Post forward substitution"<<std::endl;
				//this->linear_system->print();]
				
			}
	};
}
#endif