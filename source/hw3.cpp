#include <iostream>
#include <iomanip>
#include <thread>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../headers/PGM.hpp"

#define T double

void zeropad(const char * image
			 const int x_dimension,
			 const int y_dimension			 
			)
{
	for(int i=0; i< x)

}
void center_image_frequency()
{

}
void f2dft(){
	double * Preal = new double[M*N];
	double * Pimg  = new double[M*N];
	for(int y=0; y<N; y++){
		for(int u=0; u<M; u++){
			Preal[M*y+u] = 0;
			Pimg [M*y+u] = 0;

			for(int x =0; x < M; x++){
				Preal[M * y+u ] += f[M * y + x] * cost((2 * PI * u * x) / M);
				Pimg [M * y+ u] += - f[M * y + x] * sin((2 * PI * u * x )/ M);
			}
		}
	}
}

int main(int argc, char * argv[]){
	if(argc < 2){
		exit(-1);
	}
	int x_dimension = 0,
		y_dimension = 0,
		x_zero_padded = 0,
		y_zero_padded = 0;

	std::string file_path_original = std::string(argv[1]);
	std::string file_path_modified = std::string(argv[2]);

	std::vector<unsigned char> image;
	std::vector<unsigned char> padded;
	
	UWM::PGM().read(file_path_original, image, x_dimension, y_dimension);

	x_zero_padded = 2 * x_dimension - 1;
	y_zero_padded = 2 * y_dimension - 1;
	padded.resize(x_zero_padded * y_zero_padded);

	
	return 0;
}