#include <iostream>
#include <iomanip>
#include <thread>
#include <limits>
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
	for(int i=0; i< x_zero_padded * y_zero_padded; i++){
		padded_image[ i] =0;
	}
	for(int i=0; i< x_dimension; i++){
		for(int j=0; j < y_dimension; j++){
			padded_image[j*x_zero_padded+ i] = image[j*x_dimension+ i];
		}
	}
}

void center_image_frequency(
							std::vector<double> &padded_image,
							const int x, 
							const int y,
							const int x_zero_padded,
							const int y_zero_padded
						   )
{
	for(int i=0; i< x; i++){
		for( int j=0; j<y; j++){
			padded_image[j* x_zero_padded + i] *= pow(-1,(i+j));
		}
	}
}

void f2dft(std::vector<double> & padded_image,
		   const int M,
		   const int N,	
		   double * result,
		   bool inverse
		   )
{
	double * P_real = new double[M*N];
	double * P_img  = new double[M*N];
	double * Q_real = new double[M*N];
	double * Q_img  = new double[M*N];
	double temp = 0.0;

	double dir =  -2 * M_PI;
	if(inverse){
		dir = -dir;
	}
	//DFT Rows
	for(int y=0; y<N; y++){
		for(int u=0; u<M; u++){
			P_real[ y * M + u ] = 0;
			P_img [ y * M + u ] = 0;

			for(int x =0; x < M; x++){
				temp = (dir * u * x) / M;
				 P_real[M * y+ u] +=  padded_image[M * y + x] * cos( temp);
				// P_img [M * y+ u] += -padded_image[M * y + x] * sin(temp);
				if(inverse){
					P_img [M * y+ u] += padded_image[M * y + x] * sin(temp);
				}else{
					
					P_img [M * y+ u] += -padded_image[M * y + x] * sin(temp);
				}

				
			}
		}
	}

	double max = std::numeric_limits<double>::min();
	double min = std::numeric_limits<double>::max();
	
	//DFT Cols
	for(int x =0; x < M; x++){
		for(int v=0; v < N; v++){
			Q_real[M*v + x] = 0;
			Q_img [M*v + x] = 0;
			
			for(int y=0; y<N; y++){
				temp =  (dir * v *y) /N ;
				
				// Q_real[M * v + x ] +=   P_real[M * y + x] * cos(temp) + P_img [M * y + x] * sin(temp);
				// Q_img[M * v + x ]  +=   P_img [M * y + x] * cos(temp) - P_real[M * y + x] * sin(temp);

				if(inverse) {
					Q_real[M * v + x ] +=  P_real[M * y + x] * cos(temp) - P_img[M * y + x] * sin(temp);
					Q_img[M * v + x ]  +=  P_real[M * y + x] * sin(temp) + P_img[M * y + x] * cos(temp);
				}else{
					Q_real[M * v + x ] +=   P_real[M * y + x] * cos(temp) + P_img[M * y + x] * sin(temp);
					Q_img[M * v + x ]  += - P_real[M * y + x] * sin(temp) + P_img[M * y + x] * cos(temp);
				}
				
			} 

			if( max < (Q_real[v * M + x] + Q_img[v* M + x] ) ) {
				max =  (Q_real[v * M + x] + Q_img[v* M + x] );
			}

			if(min > (Q_real[v * M + x] + Q_img[v * M + x] ) ) {
				 min =  (Q_real[v * M + x] + Q_img[v * M + x] );
			}

			temp =   (Q_real[v * M + x] - Q_img[v * M + x] );
		
			result[v* M + x] = temp;
		
		}
	}
	std::cout<<"min:" << min <<std::endl;
	std::cout<<"max:" << max <<std::endl;

	// for(int x =0; x < M; x++){
	// 	for(int y=0; y<N; y++){
	// 		temp =(Q_real[y* M + x] - Q_img[y * M + x] );
	// 		if(inverse)
	// 		temp *= 1/ M*N;
	// 		//result[y* M + x]=  255 * ( temp - min ) / ( max - min);
	// 		result[y* M + x] = temp;
	// 	}
	// }

}

void get_G(std::vector<double> &F,int M, int N, double cuttoff,std::vector<double> &G){
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

	double temp = 0.0;
	double H= 0;

	int P = 2* M-1;
	int Q = 2* M-1;
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
			G[v* M + u]= H * F[v*M+u];
		}
	} 
}

void crop_imgage(unsigned char * cropped_imgage,
			const int x_dimension,
			const int y_dimension,
			std::vector<double> &padded_image,
			const int x_zero_padded,
			const int y_zero_padded){

	double max = std::numeric_limits<double>::min();
	double min = std::numeric_limits<double>::max();		
	unsigned char * temp = new unsigned char[x_zero_padded*y_zero_padded];
	for(int i=0; i < x_zero_padded * y_zero_padded; i++){
		if(max < padded_image[i]){
			max = padded_image[i];
		}
		if(min > padded_image[i]){
			min = padded_image[i];
		}
	}
	for(int i=0; i < x_zero_padded* y_zero_padded; i++){
		temp[i] = 255 * (padded_image[i] - min) / (max - min);
	}
	
	for(int i=0; i<x_dimension; i++){
		for( int j =0; j < y_dimension; j++){
			cropped_imgage[j*x_dimension+ i] = temp[j*x_zero_padded+ i];
		}
	}
	delete temp;
}
void print_double(
	std::string name,
	std::vector<double> &img,
	const int x,
	const int y){
	double max = std::numeric_limits<double>::min();
	double min = std::numeric_limits<double>::max();
	unsigned char * temp = new unsigned char[x*y];
	for(int i=0; i < x * y; i++){
		if(max < img[i]){
			max = img[i];
		}
		if(min > img[i]){
			min = img[i];
		}
	}
	for(int i=0; i < x * y; i++){
		temp[i] = 255 * (img[i] - min) / (max - min);
	}
	UWM::PGM().write(name,temp, x, y);
	delete temp;
}
int main(int argc, char * argv[]){
	if(argc < 4){
		std::cout<< "Useage:" << std::endl;
		std::cout<< "\t [0] Source Image abs path" << std::endl;
		std::cout<< "\t [1] Save path" << std::endl;
		std::cout<< "\t [2] d0" << std::endl;
		exit(-1);
	}
	int x_dimension = 0,
		y_dimension = 0,
		x_zero_padded = 0,
		y_zero_padded = 0;

	std::string file_path_original = std::string(argv[1]);
	std::string save_path = std::string(argv[2]);
	double d0 = atof(argv[3]);
	
	std::vector<unsigned char> image;
	std::vector<unsigned char> padded;
	std::vector<unsigned char> G;

	std::vector<unsigned char> F;

	UWM::PGM().read(file_path_original, image, x_dimension, y_dimension);

	x_zero_padded = 2 * x_dimension -1;
	y_zero_padded = 2 * y_dimension -1;
	padded.resize(x_zero_padded * y_zero_padded);
	
	G.resize(x_zero_padded * y_zero_padded);
	F.resize(x_zero_padded * y_zero_padded);

	zero_pad(image.data(),
		 x_dimension,
		 y_dimension,
		 padded.data(),
		 x_zero_padded,
		 y_zero_padded);

	std::vector<double> double_padded(padded.begin(), padded.end());
	
	print_double(save_path+"/1_zero_pad_"+std::to_string(d0)+".pgm",double_padded,x_zero_padded, y_zero_padded);
	center_image_frequency(
		double_padded,
		x_dimension,
		y_dimension,
		x_zero_padded,
		y_zero_padded);

	print_double(save_path+"/2_center_freq_"+std::to_string(d0)+".pgm",double_padded,x_zero_padded, y_zero_padded);
	//UWM::PGM().write("bin/center_freq.pgm",padded, x_zero_padded, y_zero_padded);
	double cuttoff = 255;

	
	std::vector<double> double_F;
	double_F.resize(x_zero_padded * y_zero_padded);
	
	f2dft(double_padded,
		  x_zero_padded,
		  y_zero_padded,
		  double_F.data(),
		  false
		);

	print_double(save_path+"/3_F_"+std::to_string(d0)+".pgm",double_F, x_zero_padded, y_zero_padded);
	
	// 
	// G => GP
	//
	std::vector<double> double_G;
	double_G.resize(x_zero_padded * y_zero_padded);
	//std::vector<double> double_G(double_F.begin(), double_F.end());
	get_G(double_F , x_zero_padded,	y_zero_padded,d0 , double_G);

	 print_double(save_path+"/4_G_"+std::to_string(d0)+".pgm",double_G, x_zero_padded, y_zero_padded);
	
	std::vector<double> double_GP;
	double_GP.resize(x_zero_padded * y_zero_padded);
	// 
	// G => GP
	// 
	f2dft(
		double_G,
		x_zero_padded,
		y_zero_padded,	
		double_GP.data(),
		true
	 );
	 
	
	print_double(save_path+"/5_IDFT_F{G}_"+std::to_string(d0)+".pgm",double_GP, x_zero_padded, y_zero_padded);
	 
	center_image_frequency(
		double_GP,
		x_dimension,
		y_dimension,
		x_zero_padded,
		y_zero_padded);
	print_double(save_path+"/6_undo_center_freq_"+std::to_string(d0)+".pgm",double_GP, x_zero_padded, y_zero_padded);
	image.clear();
	image.resize(x_dimension * y_dimension);
	crop_imgage( image.data(),
		 x_dimension,
		 y_dimension,
		 double_GP,
		 x_zero_padded,
		 y_zero_padded);
	UWM::PGM().write( save_path+"/7_Cropped(G)_"+std::to_string(d0)+".pgm",image, x_dimension, y_dimension);
	return 0;
}