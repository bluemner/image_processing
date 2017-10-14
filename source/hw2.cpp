#include <iostream>
#include <iomanip> 
#define _USE_MATH_DEFINES
#include <cmath>
#include "../headers/PGM.hpp"
#define T float 

void apply_mask(const int size,
                T** mask,
                const int image_height, 
                const int image_width,
                const  std::vector<unsigned char> &original,
                std::vector<unsigned char> &filtered)
{
    for(int i =0; i<image_width; i++){
        for(int j=0; j< image_height; j++){
            for(int k=0; k < i+size; k++){
                for(int l=0;l < j+size; l++){
                    filtered[j*image_width+ i] = mask[k-i][l-j] *filtered[j*image_width+ i];
                }
            }
        }
    }
}

void get_gaussian_mask(const int size, T** mask)
{
    //               5 /2 = 2; 
    int mp =(int)  size / 2;
    T sigma = 1.0;
    T r; 
    T s = 2.0 * sigma * sigma;
    T sum = 0.0;
    std::cout<< "mp:\t" <<mp<< "\t"<< (-1* mp) << std::endl;
    for(int i = (-1* mp); i<= mp; i++){
        for(int j = (-1* mp); j<= mp; j++){
            r = sqrt(i*i + j*j);
            mask[i + mp][j + mp] = (exp(-(r*r)/s))/(M_PI * s);
            sum += mask[i + mp][j + mp];
        }
    }

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            mask[i][j] /= sum;
        }
     
    }
        
}

void print(const int size, T** mask){
    for(int i=0; i< size; i++){
        for(int j=0; j<size; j++){
           std::cout<< std::setprecision(16);
           std::cout<< mask[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char * argv[]){
    if (argc < 5){
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
    }else if(mask_size % 2 == 0){
        std::cerr << "mask size should be an odd number" <<std::endl;
        return -1;
    }

    int image_width = 0, image_height = 0;
    std::vector<unsigned char> image;
    UWM::PGM().read(file_path_original, image, image_width, image_height);
    std::cout<<"Image Loaded"<< std::endl;

    // Memory allocation
    T** mask = new T*[mask_size];
    for(int i = 0; i < mask_size; i++){
        mask[i] = new T[mask_size];
        for(int j=0; j<mask_size; j++){
            mask[i][j] = (T) 0.0;
        }
    }
    
    get_gaussian_mask(mask_size, mask);

    std::vector<unsigned char> fi;
    fi.resize(image_height,image_width);
    /*
        const int size,
                const T** mask,
                const int image_height, 
                const int image_width,
                const  std::vector<unsigned char> &original,
                std::vector<unsigned char> &filtered
    */
    apply_mask(mask_size,mask,image_height, image_width,image, fi);


    print(mask_size,mask);
    // Clean up
    for(int i = 0; i < mask_size; i++){
       delete mask[i];
    }
    delete mask;

    return 0;
}