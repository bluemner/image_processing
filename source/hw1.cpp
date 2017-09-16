#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <math>
#include "../headers/PGM.h"
#include "../headers/bi_linear.hpp"
#define T double
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
	T t = 1.0;
	try{
		T t = std::stod( argv[3]);
	}catch(std::exception e){
		exit(-2);
	}
	uwm::PGM().read(file_path_original, image, x, y);
	
	betacore::point_value<T> I_00;
	betacore::point_value<T> I_01;
	betacore::point_value<T> I_10;
	betacore::point_value<T> I_11;

	// your application here
	// As an example, let's just make an inversion of the input image.
	std::vector<unsigned char> changed_image;
	int dx = (int)x * t;
	int dy = (int)y * t;
	for(size_t j = 1; j< (size_t)dy-1; j++ )
		for (size_t i = 1; i < (size_t)dx-1; i++){
			int dj = (int)(j/t);
			int di = (int)(i/t);
			I_00.x = i / t - 1 / t;
			I_00.y = j / t - 1 / t;
			I_00.value = (T)image[dj * x + di];

			I_01.x = x/  t;
			I_01.y = y/  t;
			I_01.value = (T)image[dj * x + di];

			I_10.x = x/  t;
			I_10.y = y/  t;
			I_10.value = (T)image[dj * x + i];

			I_10.x = x/  t;
			I_10.y = y/  t;
			I_10.value = (T)image[dj * x + di];

			betacore::Bi_Linear<T> bl((T)i*t,(T)y*t, I_00, I_01, I_10, I_11);
			changed_image.at(j * dx + i) = (char) bl.get_value();
		}
	

	for (size_t j = 0; j < (size_t)y; j++)
		for (size_t i = 0; i < (size_t)x; i++)
		{
			
			I_00.x = x;
			I_00.y = y;
			I_00.value = (T)image[j * x + i];


			betacore::Bi_Linear<T> bl((T)i*t,(T)y*t, I_00, I_01, I_10, I_11);
			double value =	bl.get_value();
		}
	// for (size_t j = 0; j < (size_t)delta_y; j++)
	// 	for (size_t i = 0; i < (size_t)delta_x; i++)
	// 	{
	// 		//image[j*xdim+i] = 255 - image[j*xdim+i];
	// 		I_00.x = x;
	// 		I_00.y = y;
	// 		I_00.value = (T)image[j * x + i];


	// 		betacore::Bi_Linear<T> bl((T)dx,(T)dy, I_00, I_01, I_10, I_11);
	// 		double value =	bl.get_value();
	// 	}

	uwm::PGM().write(file_path_new, changed_image, dx, dy);
	return 0;
}