#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include <cmath>
#include <algorithm>
#include "../headers/PGM.hpp"
#include "../headers/bi_linear.hpp"
#include "../headers/linear_system.hpp"
#include "../headers/matrix.hpp"
#define F float
#define T int

int interpolate(
	const T &x,
	const T &y,
	int A,
	int B,
	int C,
	int D
){	
	int P  = (int)  A*(1-x)*(1-y)+
			B *x *(1-y) +
			C *(1-x)*y +
			D * x * y;
	
		
	return P;
}

void process(std::vector<unsigned char>  &pixels, std::vector<unsigned char> &temp, int w, int h, int w2, int h2){
	int  x, y, current, op1,op2 ;
	
	betacore::point_value<int> I_00;
	betacore::point_value<int> I_01;
	betacore::point_value<int> I_10;
	betacore::point_value<int> I_11;
	
	float x_ratio = ((float)(w-1))/w2 ;
    float y_ratio = ((float)(h-1))/h2 ;
    float x_diff, y_diff, ya, yb ;
    int offset = 0 ;
    for (int i=0;i<h2;i++) {
        for (int j=0;j<w2;j++) {
            x = (int)(x_ratio * j) ;
            y = (int)(y_ratio * i) ;
            x_diff = (x_ratio * j) - x ;
            y_diff = (y_ratio * i) - y ;
            current = y*w+x ;
			
			// 
			// 	3               1
			// 	1               6        8        0
			//  +--------+--------+--------+--------+
			//  |aaaaaaaa|aaaaaaaa|aaaaaaaa|bbbbbbbb|
			//  +--------+--------+--------+--------+
			// Need to just return the b bits
			I_00.value = pixels[current] & 0xff ;
			I_00.x =x;
			I_00.y =y;
			I_10.value = pixels[current+1] & 0xff ;
			I_10.x =x+1;
			I_10.y =y;

			I_01.value = pixels[current+w] & 0xff ;
			I_01.x =x;
			I_01.y =y+1;

			I_11.value = pixels[current+w+1] & 0xff ;
			I_11.x =x;
			I_11.y =y+1;
            // betacore::Bi_Linear<float,int> bl(
			// 	x_diff,
			// 	y_diff,
			// 	I_00,
			// 	I_10,
			// 	I_01,
			// 	I_11
			// );
			// op2 = (int) bl.get_value();
			op1 = interpolate(x_diff,y_diff,I_00.value,I_10.value,I_01.value,I_11.value);
		
            temp[offset++] =op1;
        }
    }
 
}


int main(int argc, char *argv[])
{
	int w = 0, h = 0, v=0, u=0;
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
	double t = 1.0;
	try{
		 t = std::stod( argv[3]);
	}catch(std::exception e){
		exit(-2);
	}
	
	UWM::PGM().read(file_path_original, image, w, h);
	UWM::PGM().write("same.pgm", image, w, h);
	std::cout <<"Image loaded" <<std::endl;


	// your application here
	// As an example, let's just make an inversion of the input image.

	int dx = (int)w * t;
	int dy = (int)h * t;
	std::vector<unsigned char> changed_image;
	std::vector<int> temp_image;

	changed_image.resize(dy * dx);
	temp_image.resize(dy * dx);
	std::cout<<std::fixed<<std::setprecision(2)<< "(" << w <<"," << h << ") * "<< t << " => ("<< dx << "," << dy <<")" <<std::endl;
		process(image,changed_image,w,h,dx,dy);
	
	try{
		UWM::PGM().write(file_path_new, changed_image, dx, dy);
	}catch(const std::exception& e){
		
		std::cout<<"Problem\n"<< e.what()<<std::endl;
	}

	std::cout<<"done" <<std::endl;

	return 0;
}

