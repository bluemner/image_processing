#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../headers/PGM.hpp"
#define T float 

void get_gaussian_mask(const int size, T** mask){
    //               5 /2 = 2; 
    int mid_point =  size / 2;
    T sigma = 1.0;
    T r, s = 2.0 * sigma * sigma;
    for(int i = -1* mid_point; i< mid_point; i++){
        for(int j = -1* mid_point; j< mid_point; j++){
           
            r = sqrt(i*i + j*j);
            mask[i + mid_point][j + mid_point] = (exp(-(r*r)/s))/(math.M_PI * s);
            sum += mask[i + mid_point][j + mid_point];
        }
    }
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            mask[i][j] /= sum;
        }
     
    }
        
}

int main(int argc, char * argv[]){
    if (argc < 5)
	{
		std::cout << "Usage:" << std::endl;
		std::cout << "\t<file_path_original>" << std::endl;
		std::cout << "\t<file_path_new>" << std::endl;
        std::cout << "\t<mask_size>" << std::endl;
        std::cout << "\t<cpu_thread_count>"  << std::endl;
		exit(-1);
    }
    
	std::string file_path_original = std::string(argv[1]);
    std::string file_path_new = std::string(argv[2]);
    int mask_size  = std::stoi(argv[3]);
    int threads   = std::stoi(argv[4]);
    if(mask_size < 1 || threads < 1){
        std::cerr << "mask_size >=1 & threads >=1" <<std::endl;
        return -1;
    }
    int w = 0, h = 0;
    std::vector<unsigned char> image;
    UWM::PGM().read(file_path_original, image, w,h);
    std::cout<<"Image Loaded"<< std::endl;

    // Memory allocation
    T** mask = new T*[mask_size];
    for(int i = 0; i < mask_size; i++){
        mask[i] = new T[mask_size];
    }      

    //unsigned char* image_ = image.data();
    get_gaussian_mask(mask_size, mask);


    // Clean up
    for(int i = 0; i < mask_size; i++){
       delete mask[i];
    }
    delete mask;
   

    return 0;
}