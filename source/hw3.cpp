#include <iostream>
#include <iomanip>
#include <thread>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../headers/PGM.hpp"

#define T double

void zero_pad(unsigned char * image,
			 const int x_dimension,
			 const int y_dimension,
			 unsigned char * padded_image,
			 const int x_zero_padded,
			 const int y_zero_padded
			)
{
	// int shift_i = x_dimension/2-1, 
	// 	shift_j = y_dimension/2-1,
	// 	x = 0,
	// 	y = 0;
	// int s_00 = shift_i;
	// int s_01 = s_00 + x_dimension;
	// int s_10 = shift_j;
	// int s_11 = s_10 + y_dimension;

	// char value = 0;
	// for(int i=0; i< x_zero_padded; i++){
	// 	for( int j=0; j<y_zero_padded; j++){
			
	// 		if(i > s_00 && i < s_01 && 
	// 		   j > s_10 && j < s_11)
	// 		{
	// 		  x = i- shift_i;
	// 		  y = j- shift_j;
	// 		  value = image[y* x_dimension + x];
	// 		}else{
	// 			value =0;
	// 		}
	// 		padded_image[j* x_zero_padded + i] = value  ;
	// 	}
	// }
	for(int i=0; i< x_dimension; i++){
		for(int j=0; j < y_dimension; j++){
			padded_image[j*x_zero_padded+ i] = image[j*x_dimension+ i];
		}
	}
}
void center_image_frequency(unsigned char * image,
							const int x_dimension, 
							const int y_dimension,
							unsigned char * padded_image,
							const int x_zero_padded,
							const int y_zero_padded
							 )
{
	
	for(int i=0; i< x_zero_padded; i++){
		for( int j=0; j<y_zero_padded; j++){
			
			padded_image[j* x_zero_padded + i] *= (char) pow(-1,(i+j));
		}
	}
}
void f2dft(char * image,
		   const int M,
		   const int N,
		   const int P,
		   const int Q,
		   const double cuttoff,
		   char * result
		   )
{
	double * Preal = new double[M*N];
	double * Pimg  = new double[M*N];
	double * Qreal = new double[M*N];
	double * Qimg  = new double[M*N];
	for(int y=0; y<N; y++){
		for(int u=0; u<M; u++){
			Preal[M*y+u] = 0;
			Pimg [M*y+u] = 0;

			for(int x =0; x < M; x++){
				Preal[M * y+u ] += image[M * y + x] * cos((2 * M_PI * u * x) / M);
				Pimg [M * y+ u] += - image[M * y + x] * sin((2 * M_PI * u * x )/ M);
			}
		}
	}
	for(int x =0; x < M; x++){
		for(int v=0; v < N; v++){
			Qreal[N*x+v] = 0;
			Qimg [N*x+v] = 0;
			for(int y=0; y<N; y++){
				Qreal[M * y+v ] += Preal[M*y+x] * cos((2 * M_PI * v * y) / N) + sin((2 * M_PI * v * y)/ N );
				Qimg[M * y+v ]  += Pimg [M*y+x] * sin((2 * M_PI * v * y)/ N ) + cos((2 * M_PI * v * y) / N);
			}

		}
	}
	double temp =0.0;
	char H =0;
	for(int u =0; u < M ; ++u ){
		for(int v=0; v < N ; v++){
		    temp = (u-P/2)*(u-P/2) + 
		   			(v-Q/2) *(v-Q/2);
			temp =sqrt(temp);
			
			if(temp <= cuttoff){
				H = 1;
			}else {
				H = 0;
			}
			H * (Qreal[v* P + u] + Qimg[v* P + u] );
		}
	}

}
void G(){
	/*
		D_uv => D(U,V)  = [ (u-P/2)^2 + (v-Q/2)^2]^(1/2)
		D_0 :: is the cut off frequency.
		
		H_uv ::
		
		if(D_uv < D_0){
			H_uv =1
		}else {
			H_uv 0
		}

		G = g(x,y) = F^-1{H_uv * F_uv}
	*/
}
void g_prime(){
	/*
		IDFT{G} => g*

		g* (-1)^(x+y)=> g'
		cropping g' (x_dim,y_dim)

	 */
	
}
void cropp_imgage(unsigned char * cropped_imgage,
			const int x_dimension,
			const int y_dimension,
			unsigned char * padded_image,
			const int x_zero_padded,
			const int y_zero_padded){
	for(int i=0; i<x_dimension; i++){
		for( int j =0; j < y_dimension; j++){
			cropped_imgage[j*x_dimension+ i] = padded_image[j*x_dimension+ i];
		}
	}
}
int main(int argc, char * argv[]){
	if(argc < 3){
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

	x_zero_padded = 2 * x_dimension;
	y_zero_padded = 2 * y_dimension;
	padded.resize(x_zero_padded * y_zero_padded);

	zero_pad(image.data(),
		 x_dimension,
		 y_dimension,
		 padded.data(),
		 x_zero_padded,
		 y_zero_padded);

	UWM::PGM().write("zero_pad.pgm",padded, x_zero_padded, y_zero_padded);
	center_image_frequency(image.data(),
		x_dimension,
		y_dimension,
		padded.data(),
		x_zero_padded,
		y_zero_padded);
	UWM::PGM().write("center_freq.pgm",padded, x_zero_padded, y_zero_padded);


	return 0;
}