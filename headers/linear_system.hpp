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
#ifndef _BETACORE_LINEAR_SYSTEM_H_
#define _BETACORE_LINEAR_SYSTEM_H_

#ifndef _BETACORE_MATRIX_H_
#include "./matrix.hpp"
#endif
#include <exception>
#include <iostream>
#include <iomanip>
namespace betacore{
	struct Linear_System_Exception : public std::exception {
		const char * what () const throw () {
			return "Linear System Exception";
		}
	};

	template <typename T>
	class Linear_System{
		private:
			Matrix<T> * A;
			Matrix<T> * L;
			Matrix<T> * U;
			size_t size;
			T* x;
			T* b;
			template<size_t rows, size_t cols>
			void copy(T (&A)[rows][cols], T (&x)[rows], T (&b)[rows]){
				this->size= rows;
				this->A = new Matrix<T>(A);
				this->x = new T[rows];
				this->b = new T[rows];
				size_t i=0;
				//vectors have the same size;
				for(; i< rows; i++){
					this->x[i] = x[i];
					this->b[i] = b[i];
				}
			}
			void print(Matrix<T> *M){
				size_t i = 0;
				size_t j = 0;
				size_t precision = 5;
				size_t width = 10 + precision;
				char fill =' ';
				std::cout<< "┌";
				for(i=0; i< this->size; i++){
					std::cout<<" ";
					for(j=0; j< width; ++j){
						std::cout<<" ";
					}
				}
				std::cout<<" ┐   ┌ ";
				for(j=0; j< width; ++j){
						std::cout<<" ";
					}
				std::cout<<" ┐   ┌ ";
				for(j=0; j< width; ++j){
						std::cout<<" ";
					}		
				std::cout<<" ┐"<<std::endl;

				for(i=0; i< this->size; i++){
					
						std::cout<<"│";
					
					for(j=0; j< this->size; j++){
						std::cout<<" "<<std::fixed<<std::setprecision(precision)<<std::setfill(fill)<<std::setw(width);
						std::cout<<M->at(i,j);
					}					
					
					std::cout<<" │";
					if( i == (this->size/2)   ){
						std::cout<<" X "<<"│"<<" "<<std::setfill(fill) << std::setw(width)<<this->x[i]<<" "<<"│";
						std::cout<<" = "<<"│"<<" "<< std::setw(width)<< this->b[i]<<" "<<"│";
					}else{
						std::cout<<"   "<<"│"<<" "<< std::setw(width)<< this->x[i]<<" "<<"│";
						std::cout<<"   "<<"│"<<" "<< std::setw(width)<< this->b[i]<<" "<<"│";
					}
							
					std::cout<<std::endl;
				}

				std::cout<< "└";
				for(i=0; i< this->size; i++){	
					std::cout<<" ";				
					for(j=0; j< width; ++j){
						std::cout<<" ";
					}	
				}
				std::cout<<" ┘   └ ";
				for(j=0; j< width; ++j){
						std::cout<<" ";
					}	
				std::cout<<" ┘   └ ";
				for(j=0; j< width; ++j){
						std::cout<<" ";
					}	
				std::cout<<" ┘"<<std::endl;
				for(i=0; i< this->size; i++){
					std::cout << "("<<x[i]<<","<<b[i]<<")";
				}
				std::cout<<std::endl;

			}
		public:
			Linear_System(){}

			template<size_t rows, size_t cols>
			Linear_System(T (&A)[rows][cols],T (&x)[rows],T (&b)[rows]){
				if(rows != cols){
					throw Linear_System_Exception();	
				}
				copy(A,x,b);
				T L[rows][cols];
				T U[rows][cols];
				Matrix<T>::lu(A,L,U);
				this->L = new Matrix<T>(L);
				this->U = new Matrix<T>(U);
			}
			
			Linear_System(Matrix<T> &A, Matrix<T> &x,Matrix<T> &b){
			//	A.print();
				int size= A.row_count();
				int col = A.col_count();
				Matrix<T> _L(size, col);
				Matrix<T> _U(size, col);
				A.lu(_L,_U);
			

				this->L = new Matrix<T>(_L);
				this->U = new Matrix<T>(_U);
				
				this->A = new Matrix<T>(A);
		//		this->A->print();
				this->size = A.row_count();
				this->x = new T[size];
				this->b = new T[size];
				
				size_t i=0;
				//vectors have the same size;
				for(; i< A.row_count(); i++){
					this->x[i] = 0;// x.at(i,0);//[i];
					this->b[i] = b.at(i,0);
				}

				crout();
			}
			~Linear_System(){
				delete this->A;
				this->A = nullptr;
				
				delete this->L;
				this->L = nullptr;

				delete this->U;
				this->U = nullptr;

				delete this->x;
				this->x = nullptr;

				delete this->b;
				this->b = nullptr;
			}
			
			/*
				
			*/
			void forward_substitution(){
				int i=0;
				int j=0;
				size_t n = this->size;
				for ( j=0; j < n ; j++ ){
					size_t uj = (size_t) j;
					T Ajj=this->A->at(uj,uj);
					if (Ajj  == 0){
						break;
					}
					this->x[j] = this->b[j] / Ajj;
					for (i=j+1 ; i<(int)n  ; i++  ){
						size_t ui = (size_t) i;
						this->b[i]= this->b[i] -  this->A->at(ui,uj) * this->x[j];
					}
				}
			}
			void crout(){
				this-> size;
				this->L->erase_1();
			//	this->A->print();
				for(int k=0;k< this-> size; ++k){
					for(int i=k; i<this-> size; ++i){
						T sum=(T)0;
						for(int p=0;p<k;++p)
						{
							sum+= ((T) L->at(i,p) ) *  ((T)L->at(p,k));
							
						}
						T value  = A->at(i,k)-sum;
					
						L->set((unsigned)i,k,  value ); // not dividing by diagonals
					}
					for(int j=k+1;j<this-> size;++j){
						double sum=0;
						for(int p=0;p<k;++p)
						{
							sum+=L->at(k,p)*L->at(p,j);
						}
						L->set( (unsigned) k,j, ((A->at(k,j)-sum)/L->at(k,k)));
					}
				}
			}

			/*
			 Solves using LU factorization and stores the result into x
			 not this processes will change the state of all parts of the
			 linear system. 
			*/
			void solveCrout(){
				T * y = new T[this->size];
				//this->L->print();
				for(size_t i=0;i<this-> size;++i){
					T sum=0;
					for(size_t k=0;k<i;++k)
					{
						sum+=L->at(i,k) *y[k];
					}
					y[i]=(b[i]-sum)/L->at(i,i);
				}

				for(int i=this->size-1; i>=0 ;--i){
					T sum=0;
					for(size_t k=i+1; k < this->size; ++k)
					{
						sum+=L->at((size_t)i,k)*this->x[k];
					}
					T temp = (T)y[i] - sum;
					this->x[i]=temp; // not dividing by diagonals
				}
				delete y;
				y= nullptr;
			}
			void forward_substitution_l(){
				int i=0;
				int j=0;
				//this->L->print();
				size_t n = this->size;
				for ( j=0; j < n ; j++ ){
					size_t uj = (size_t) j;
					T Ajj=this->L->at(uj,uj);
					if (Ajj  == 0){
						break;
					}
					this->x[j] = this->b[j] / Ajj;
					for (i=j+1 ; i<(int)n  ; i++  ){
						size_t ui = (size_t) i;
						this->b[i]= this->b[i] -  this->L->at(ui,uj) * this->x[j];
					}
				}
			}

			/*

			*/
			void backward_substitution( ){
				int n = (int) this->size;
				int i=0;
				int j=this->size-1;
				for ( ; j >=0 ; j-- ){
					size_t uj = (size_t) j;
					T Ajj=this->A->at(uj,uj);
					if (Ajj == 0.0){
						break; 
					}
					T xj = this->b[j] / Ajj;
					this->x[j] =xj;				
					for (i=0; i<=(int)j-1 ; i++  ){
						size_t ui = (size_t) i;
						this->b[i]= this->b[i] -  this->A->at(ui,uj) * this->x[j];
					}
					
				}
			}
			template<size_t size>
			void Ly_b(const T(&x)[size], T (&result)[size]){
				int n = this->size-1;
				T sum;
				for(int i=0; i< n; i++) {
					sum=0;					
					for(int j=i+1; j<i; j++){
						sum += this->A.at(i,j)*result[j];
					}
					result[i]=(x[i]*this->A.at(0,i)) - sum;
				}
			}
			/*
			*/
			template<size_t size>
			void Rx_y(const T (&x)[size], T (&result)[size]){
				
				int n = this->size-1;
				T sum;
				for(int i=this->size-1; i>=0; i--) {
					sum=0;					
					for(int j=i+1; j<=n; j++){ 
						sum += this->A.at(i,j)*result[j];
					}
					result[i]= ( x[i] - sum ) / this->A.at(i,i);
				}
			}
			T value(T x, T XL,T XR){
				         //a0  + a1*(x-XL)+ a2*(x-XL)^2 + a3 * (x-XL)^2(x-XR)
						 
						 T a0 = this->x[0];
						 T a1 = this->x[1]*(x-XL);
						 T a2 = this->x[2]*(x-XL)*(x-XL);
						 T a3 = this->x[3]*(x-XL)*(x-XL)*(x-XR);
						 T total = a0 + a1 + a2 + a3;
				return (T) (total);
			}
			void get_x(Matrix<T> &x){
				//Matrix<T> mm(this->size,1);
				for(size_t i=0; i< this->size; i++){
					x.set(i,0,this->x[i]);
				} 
			
			}
			
			template<size_t row>
			void get_x(T(&x)[row]){
				for(size_t i=0; i< this->size; i++){
					x[i] =this->x[i];
				}
			}
			
			T SL(){
				return x[1];
			}
			/*
				Printing linear system 
			 */
			void print(){
				std::cout<<"A:"<<std::endl;
				print(this->A);
				// std::cout<<"L:"<<std::endl;
				// print(this->L);
				// std::cout<<"U:"<<std::endl;
				// print(this->U);
			}
			
	};
}



#endif