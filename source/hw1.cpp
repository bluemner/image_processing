#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include <cmath>
#include <algorithm>
#include "../headers/PGM.hpp"
#include "../headers/bi_linear.hpp"

#define T double
#define PIXEL_RANGE(l, min, max)if (l < min) { l = min; } else if (l > max) { l = max; } 
int main(int argc, char *argv[])
{
	int x = 0, y = 0;
	std::vector<unsigned char> image;
	if (argc < 4)
	{
		std::cout << "Usage:" << std::endl;
		std::cout << "\tPlease Provide" << std::endl;
		std::cout << "\t<file_path_original>" << std::endl;
		std::cout << "\t<file_path_new>" << std::endl;
		std::cout << "\t<t>\tscaling size" << std::endl;
		exit(-1);
	}
	std::string file_path_original = std::string(argv[1]);
	std::string file_path_new = std::string(argv[2]);
	//replace( file_path_new.begin(), file_path_new.end(), '\\', '/' );
	T t = 1.0;
	try{
		 t = std::stod( argv[3]);
	}catch(std::exception e){
		exit(-2);
	}
	
	
	UWM::PGM().read(file_path_original, image, x, y);
	UWM::PGM().write("same.pgm", image, x, y);
	std::cout <<"Image loaded" <<std::endl;

	betacore::point_value<T> I_00;
	betacore::point_value<T> I_01;
	betacore::point_value<T> I_10;
	betacore::point_value<T> I_11;

	// your application here
	// As an example, let's just make an inversion of the input image.

	int dx = (int)x * t;
	int dy = (int)y * t;
	std::vector<unsigned char> changed_image;
	changed_image.resize(dy * dx);	
	std::cout<<std::fixed<<std::setprecision(2)<< "(" << x <<"," << y << ") * "<< t << " => ("<< dx << "," << dy <<")" <<std::endl;

	for(int j = 0; j< dy; j++ ){
		T v = T(j) / T( dy-1 );
		for (int i = 0; i < dx; i++){
			T u = T(x) / T(dx-1);
			
			I_00.x = (int) (u * dx) + 0;
			I_00.y = (int) (v * dy) + 0;
			PIXEL_RANGE( I_00.x ,0,dx-1);
			PIXEL_RANGE( I_00.y ,0,dy-1);
			I_00.value = image[((int)I_00.y ) *x + ((int) I_00.x)];


			I_01.x = (int) (u * dx) + 0;
			I_01.y =(int) (v * dy)  + 1;
			PIXEL_RANGE( I_01.x ,0,dx-1);
			PIXEL_RANGE( I_01.y ,0,dy-1);
			I_01.value= image[((int)I_01.y ) *x + ((int) I_01.x)];

			I_10.x =(int) (u * dx) + 1;
			I_10.y =(int) (v * dy)  + 0;
			PIXEL_RANGE( I_10.x ,0,dx-1);
			PIXEL_RANGE( I_10.y ,0,dy-1);
			I_10.value= image[((int)I_10.y ) *x + ((int) I_10.x)];

			
			I_11.x =(int) (u * dx) + 1;
			I_11.y = (int) (v * dy)  + 0;
			PIXEL_RANGE( I_11.x ,0,dx-1);
			PIXEL_RANGE( I_11.y ,0,dy-1);
			I_11.value= image[((int)I_11.y ) *x + ((int) I_11.x)];

			//std::cout<<std::fixed<<std::setprecision(2)<< "(" << i <<"," << j << ")" <<std::endl;
			betacore::Bi_Linear<T> bl
			(
				i,
				j,
				I_00,
				I_01,
				I_10,
				I_11
			);
			
			char op1 =  (unsigned char) (
				   I_00.value * (1-i)*(1-j)+ 
				   I_10.value * i*(1-j) + 
				   I_01.value *(1-i)*j + 
				   I_11.value * i*j
				);  
		
			char op2 = (char) bl.get_value();
	
			

	
			changed_image[j * dx + i]= op1; // image[((int)old_y) *x + ((int) old_x)];
									
			//image[j*dx+i];//
			//
			
		}
	}

	
		try{
		UWM::PGM().write(file_path_new, changed_image, dx, dy);
	}catch(const std::exception& e){
		
		std::cout<<"Problem\n"<< e.what()<<std::endl;
	}

	std::cout<<"done" <<std::endl;

	return 0;
}

