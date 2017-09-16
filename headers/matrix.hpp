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
#ifndef _BETACORE_MATRIX_H_
#define _BETACORE_MATRIX_H_
#include <iostream>
#include <exception>
#include <iomanip>
namespace betacore
{
struct MatrixException : public std::exception {
   const char * what () const throw () {
      return "Matrix Exception";
   }
};
/**
 * @desc instance of a NxM matrix
 */
template <typename T>
class Matrix
 {
  	private:
		T **M;
		unsigned int _rows;
		unsigned int _cols;
		template<size_t rows, size_t cols>
		void copy(T (&M)[rows][cols] ){
			this->M = new T*[rows];
			this->_rows = rows;
			this->_cols = cols;
			for(int i = 0; i < rows; ++i)
				this->M[i] = new T[cols];
			for(size_t i = 0; i < rows; ++i)
			{
				for(size_t j = 0; j < cols; ++j){
					this->M[i][j] = M[i][j];
				}
			}
		}
		void copy(T** M,const int rows, const int cols){
			this->_rows=rows;
			this->_cols=cols;
			this->M =new T*[rows];
			for(int i=0; i<rows;++i)
				this->M[i]=new T[cols];
			for(size_t i =0; i< rows; ++i)
			{
				for(size_t j=0; j<cols; ++j){
					this->M[i][j] = M[i][j];
				}
			}
		}
		T** identity(unsigned int size){
			T** t  =new T*[size];
			for(size_t i=0; i<size;++i)
				t[i]=new T[size];
			for(size_t i=0; i< size; ++i){
				for(size_t j=0; j < size; ++j){
					t[i][j]= (i==j)?1:0;
				}
			}
			return t;
		}
	public:
  	/*
	 * Create null matrix
	 */
	Matrix(){
		this->M = nullptr;
		this->_rows = -1;
		this->_cols = -1;
	}
	Matrix(Matrix &m){
		this->M = nullptr;
		// this->_rows = -1;
		// this->_cols = -1;
		 *this = m;
		int rows = m._rows;
		int cols = m._cols;

		// this->M = new T*[rows];
		// for(size_t i=0; i<rows;++i)
		// 	this->M[i]=new T[cols];
		// for(size_t i = 0; i < rows; ++i)
		// {
		// 	for(size_t j = 0; j < 1; ++j){
		// 		this->M[i][j] = m.M[i][j];
		// 	}
		// }
		// this->_rows = rows;
		// this->_cols = cols;
	}
	/**
	 * @desc
	 * @param 
	 */
	template<size_t rows>
	Matrix(T (&matrix) [rows]){
			this->M = new T*[rows];
			this->_rows = rows;
			this->_cols = 1;
			for(int i = 0; i < rows; ++i)
				this->M[i] = new T[1];
			for(size_t i = 0; i < rows; ++i)
			{
				this->M[i][0] =matrix[i];
			}
	}
	/**
	 * @desc Creates an instace of MxN Matrix of Type <T>
	 * @param {T} matrix with type <T> and size = rows x cols
	 */
	template<size_t rows, size_t cols>
	Matrix(T (&matrix) [rows][cols]){
		copy(matrix);
	}

	/*
	 * @desc Create identity matrix
	 * @param {int} size Create identity matrix of M x M st. M = Size
	 */
	Matrix(const unsigned int rows, const unsigned int cols){
		this->M = new T*[rows];
		for(size_t i=0; i<rows;++i)
			this->M[i]=new T[cols];
		this->_rows = rows;
		this->_cols = cols;
	}
	/*
	 * @desc Create identity matrix
	 * @param {int} size Create identity matrix of M x M st. M = Size
	 */
	Matrix(const unsigned int size){
		auto id = identity(size);
		copy(id,size,size);
		delete id;
		id = nullptr;
	}
	/**
	 * Deconstructor for matrix
	 */
	~Matrix(){
		for(int i=0; i< this->_rows; i++){
			delete [] this->M[i];
			this->M[i]=nullptr;
		}
		delete [] this->M;
		this->M = nullptr;
					
		this->_rows =0;
		this->_cols =0;
	}
	/**
	 * @desc prints out matrix to terminal
	 */
	void print(){
		size_t i = 0;
		size_t j = 0;
		size_t precision = 5;
		size_t width = 10 + precision;
		char fill =' ';
		std::cout<< "┌";
		for(i=0; i< this->_cols; i++){
			std::cout<<" ";
			for(j=0; j< width; ++j){
				std::cout<<" ";
			}

		}
		std::cout<<" ┐"<<std::endl;

		for(i=0; i< this->_rows; i++){
			std::cout<<"│";

			for(j=0; j< this->_cols; j++){
				std::cout<<" "<<std::fixed<<std::setprecision(precision)<<std::setfill(fill)<<std::setw(width);
				std::cout<<this->M[i][j];
			}
			std::cout<<" │";

			std::cout<<std::endl;
		}
		std::cout<< "└";
		for(i=0; i< this->_cols; i++){
			std::cout<<" ";
			for(j=0; j< width; ++j){
				std::cout<<" ";
			}
		}
		std::cout<<" ┘"<<std::endl;

		std::cout<<std::endl;

	}
	/**
	 *
	 */
	void transpose(){
		T** transpose = new T*[this->_cols];
		for (unsigned int j =0; j< this->_cols; ++j)
			transpose[j] = new T[this->_rows];
		for( unsigned int i=0; i< this->_rows; ++i){
			for( unsigned int j=0; j < this->_cols; ++j){
				transpose[j][i] = this->M[i][j];
			}
		}
		for (unsigned int j =0; j< this->_cols; ++j)
			 delete [] this->M[j];
		delete [] this->M;
		this->M = transpose;
		auto temp = this->_rows;
		this->_rows = this->_cols;
		this->_cols = temp;
	}

	/*
	 * Matrix row Count
	 */
	unsigned int row_count(){
		return this->_rows;
	}
	/*
	 * Matrix Column Count
	 */
	unsigned int col_count(){
		return this->_cols;
	}

	T at(size_t &row, size_t &col){
		if( (unsigned int) row > this->_rows -1 ){
			throw MatrixException();
		}
		if( (unsigned int) col >  this->_cols -1){
			throw MatrixException();
		}
		return this->M[row][col];
	}
	 T at(unsigned int row , unsigned int col){
		if( (unsigned int) row > this->_rows -1 ){
			throw MatrixException();
		}
		if( (unsigned int) col >  this->_cols -1){
			throw MatrixException();
		}
		return this->M[row][col];
	}

	void set(size_t row, size_t col, T val){
		if( (unsigned int) row > this->_rows -1 ){
			throw MatrixException();
		}
		if( (unsigned int) col >  this->_cols -1){
			throw MatrixException();
		}
		this->M[row][col]= val;
	}
	void set(unsigned int  row, unsigned int col, T val){
		if( (unsigned int) row > this->_rows -1 ){
			throw MatrixException();
		}
		if( (unsigned int) col >  this->_cols -1){
			throw MatrixException();
		}
		this->M[row][col]= val;
	}
	template<size_t size>
	void set(size_t row,T (&A)[size] ){
			this->M[row]= A;
	}
	
	void lu( Matrix<T> &L, Matrix<T> &U){
		int size = this->_rows;
		size_t i=0;
		size_t j=0;
		size_t k=0;

		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
			{
				if (j < i)
					L.M[j][i] = 0;
				else
				{
					L.M[j][i] = this->M[j][i];
					for (k = 0; k < i; k++)
					{
						L.M[j][i] = L.M[j][i] - L.M[j][k] * U.M[k][i];
					}
				}
			}
			for (j = 0; j < size; j++)
			{
				if (j < i)
					U.M[i][j] = 0;
				else if (j == i)
					U.M[i][j] = 1;
				else
				{
					U.M[i][j] = this->M[i][j] / L.M[i][i];
					for (k = 0; k < i; k++)
					{
						U.M[i][j] = U.M[i][j] - ((L.M[i][k] * U.M[k][j]) / L.M[i][i]);
					}
				}
			}
		}
	}

	template <size_t size>
	static void lu(T (&A)[size][size], T (&L)[size][size], T (&U)[size][size]){
		size_t i=0;
		size_t j=0;
		size_t k=0;

		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
			{
				if (j < i)
					L[j][i] = 0;
				else
				{
					L[j][i] = A[j][i];
					for (k = 0; k < i; k++)
					{
						L[j][i] = L[j][i] - L[j][k] * U[k][i];
					}
				}
			}
			for (j = 0; j < size; j++)
			{
				if (j < i)
					U[i][j] = 0;
				else if (j == i)
					U[i][j] = 1;
				else
				{
					U[i][j] = A[i][j] / L[i][i];
					for (k = 0; k < i; k++)
					{
						U[i][j] = U[i][j] - ((L[i][k] * U[k][j]) / L[i][i]);
					}
				}
			}
		}
	}

	//
	//
	//
	void erase(){
		for(unsigned int i=0; i< this->_rows; ++i){
			for(unsigned int j=0; j< this->_cols; ++j){
				this->M[i][j]= (T)0.0;
			}
		}
	}

	void erase_1(){
		for(unsigned int i=0; i< this->_rows; ++i){
			for(unsigned int j=0; j< this->_cols; ++j){
				this->M[i][j]= (T)1.0;
			}
		}
	}

	#pragma region Region_operator
	
		Matrix& operator+=(const Matrix& rhs){

			T** R = new T*[this->_rows];
			for(unsigned int i=0; i < this->_rows; ++i){
				R[i]= new T[this->_cols];
			}
			for (unsigned int j = 0; j < this->_rows; j++)
				for (int i = 0; i < this->_cols; i++)
					R[j][i] = M[j][i] + rhs.M[j][i];
			for (unsigned int j =0; j< this->_cols; ++j)
				delete [] this->M[j];
			delete [] this->M;
			this->M = R;
			return * this;
		}

		Matrix& operator+(const Matrix& rhs){
			Matrix<T> * R = new Matrix<T>(this->_rows,this->_cols);
			R->_cols = rhs._cols;
			R->_rows = rhs._rows;
			// for(unsigned int i=0; i < this->_rows; ++i){
			// 	R[i]= new T[this->_cols];
			// }
			for (unsigned int j = 0; j < this->_rows; j++)
				for (unsigned int i = 0; i < this->_cols; i++)
					R->M[j][i] = M[j][i] + rhs.M[j][i];
			return *R;
		}

		Matrix& operator-=(const Matrix& rhs){
			if(this->_rows != rhs._rows || this->_cols != rhs._cols){
				throw MatrixException();
			}
			T** R = new T*[this->_rows];
			for(unsigned int i=0; i < this->_rows; ++i){
				R[i]= new T[this->_cols];
			}
			for (unsigned int j = 0; j < this->_rows; j++)
				for (int i = 0; i < this->_cols; i++)
					R[j][i] = M[j][i] - rhs.M[j][i];
			for (unsigned int j =0; j< this->_cols; ++j)
				delete [] this->M[j];
			delete [] this->M;
			this->M = R;
			return * this;
		}

		Matrix& operator-(const Matrix& rhs){
			if(rhs._cols != this->_cols || rhs._rows != this->_rows){
				std::cerr<<  "You cant subtract two diffent size matrices"<< "\n";
				throw MatrixException();
			}
			Matrix<T> * R = new Matrix<T>(this->_rows,this->_cols);
			R->_cols = rhs._cols;
			R->_rows = rhs._rows;
			// for(unsigned int i=0; i < this->_rows; ++i){
			// 	R[i]= new T[this->_cols];
			// }
			for (unsigned int i = 0; i < this->_rows; i++){
				for (unsigned int j = 0; j < this->_cols; j++){
					R->M[i][j] = this->M[i][j] - rhs.M[i][j];
				}
			}
			return *R;
		}

		Matrix& operator*(const T scale){
			Matrix<T> * A = new Matrix<T>(this->_rows, this->_cols);
			for (int j = 0; j < this->_rows; j++)
				for (int i = 0; i < this->_cols; i++)
					A->M[j][i] = M[j][i] * scale;
			return *A;
		}

		Matrix& operator*=(const Matrix& rhs){
			if(this->_cols != rhs._rows ){
				throw MatrixException();
			}
			
			T** R = new T*[this->_rows];
			for(unsigned int i=0; i < this->_rows; ++i){
				R[i]= new T[rhs._cols];
			}
			for(unsigned int i= 0; i< this->_rows; i++ ){
				for(unsigned int j= 0; j< rhs._cols; j++ ){
					R[i][j]=(T)0.0;
					for(unsigned int k = 0; k< this->_cols; k++ ){
						R[i][j] += M[i][k] * rhs.M[k][j];
					}
				}
			}
			this->_cols = rhs._cols;
			delete this->M;
			this->M = R;
			return *this;
		}
		Matrix& operator*(const Matrix& rhs){
			Matrix<T> * R = new Matrix<T>(this->_rows,rhs._cols);
			R->_cols = rhs._cols;
			R->_rows = this->_rows;

			for(unsigned int i=0; i < this->_rows; ++i){
				R->M[i]= new T[rhs._cols];
			}
			for(unsigned int i= 0; i< this->_rows; i++ ){
				for(unsigned int j= 0; j< rhs._cols; j++ ){
					R->M[i][j]=(T)0.0;
					for(unsigned int k = 0; k< this->_cols; k++ ){
						R->M[i][j] += M[i][k] * rhs.M[k][j];
					}
				}
			}
			return *R;
		}
		Matrix* operator*(const Matrix *rhs){
			Matrix<T> * R = new Matrix<T>(this->_rows,rhs->_cols);
			R->_cols = rhs->_cols;
			R->_rows = this->_rows;

			for(unsigned int i=0; i < this->_rows; ++i){
				R->M[i]= new T[rhs->_cols];
			}
			for(unsigned int i= 0; i< this->_rows; i++ ){
				for(unsigned int j= 0; j< rhs->_cols; j++ ){
					R->M[i][j]=(T)0.0;
					for(unsigned int k = 0; k< this->_cols; k++ ){
						R->M[i][j] += M[i][k] * rhs->M[k][j];
					}
				}
			}
			return R;
		}
		T& operator[](int row){
			return *this->M[row];
		}

		Matrix& operator=(const Matrix& rhs) {
			if (this->_rows * this->_cols != rhs._rows * rhs._cols || this->M == nullptr) {
				if(this->M != nullptr){
					for(int i=0; i< this->_rows; i++){
						delete [] this->M[i];
						this->M[i]=nullptr;
					}
					delete [] this->M;
				}
				this->_rows = rhs._rows;
				this->_cols = rhs._cols;
				this->M = new T*[this->_rows];
				for(unsigned int i=0; i < this->_rows; ++i){
					this->M[i] = new T[this->_cols];
				}
				
			}
			for (unsigned int i = 0; i < _rows; i++){
				for (unsigned int j = 0; j < _cols; j++){
					this->M[i][j] = rhs.M[i][j];
				}
			}
			return *this;
		}

		Matrix* operator=(const Matrix* rhs) {
			if (this->_rows * this->_cols != rhs->_rows * rhs->_cols || this->M == nullptr) {
				if(this->M != nullptr){
						for(int i=0; i< this->_rows; i++){
						delete [] this->M[i];
						this->M[i]=nullptr;
					}
					delete [] this->M;
				}
				this->_rows = rhs->_rows;
				this->_cols = rhs->_cols;
				this->M = new T*[this->_rows];
				for(unsigned int i=0; i < this->_rows; ++i){
					this->M[i] = new T[this->_cols];
				}
				
			}
			for (unsigned int i = 0; i < _rows; i++){
				for (unsigned int j = 0; j < _cols; j++){
					this->M[i][j] = rhs->M[i][j];
				}
			}
			return this;
		}
	#pragma endregion Region_operator
};
}
#endif