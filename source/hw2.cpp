#include <iostream>
#include <iomanip>
#include <thread>
#define _USE_MATH_DEFINES
#include <cmath>
#include "../headers/PGM.hpp"
#define T double

void apply_mask(const int segment_start, // start of segment 
    const int segment_size, // segment size
    const int mask_size, // size of mask
    T** mask, // mask
    const int y_dimension, // image height
    const int x_dimension, // image width
    unsigned char * original,
    unsigned char * filtered)
{
    int k_shift = 0;
    int l_shift = 0;
    T temp_0 =0;
    T temp_1 =0;
    T max = 0;
    T min =0;
    for(int i =0; i<x_dimension; i++){
        for(int j=segment_start; j< segment_size; j++){
             for(int k=0; k <  mask_size; k++){
                for(int l=0;l <  mask_size; l++){

                    //filtered[j*image_width+ i] = mask[k-i][l-j] *filtered[j*image_width+ i];
                    k_shift = i - k / 2;
                    l_shift = j - l / 2;
                    if( k_shift < 0 || k_shift >= x_dimension){
                        k_shift=0;
                    }
                    if( l_shift < 0 || l_shift >=y_dimension ){
                        l_shift=0;
                    }
                    filtered[j*x_dimension+ i] += mask[l][k] * original[l_shift*x_dimension+k_shift ];
                    temp_0 += mask[l][k] * original[l_shift*x_dimension+k_shift ];
                    temp_1 += mask[l][k];
                
                }
            }

            filtered[j*x_dimension+ i] =  (unsigned char) (temp_0 / temp_1); ///
            temp_0 =0;
            temp_1 =0;
            if(i+1 == x_dimension && j+1==segment_size )
                std::cout<<"Done with Row::"<< j << std::endl;
        }

    }
    std::cout << "Exiting thread" <<std::endl;
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
    int thread_count   = std::stoi(argv[4]);
    
    if(mask_size < 1 || thread_count < 1){
        std::cerr << "mask_size >=1 & thread_count >=1" <<std::endl;
        return -1;
    }else if(mask_size % 2 == 0){
        std::cerr << "Mask size should be an odd number" <<std::endl;
        return -1;
    }

    int x_dimension = 0, y_dimension = 0;
    std::vector<unsigned char> image;
    UWM::PGM().read(file_path_original, image, x_dimension, y_dimension);
    unsigned char * test_image= image.data();
    //UWM::PGM().read(file_path_original, test_image, x_dimension, y_dimension);
       
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
    print(mask_size,mask);
    
    unsigned char * fi = new unsigned char[x_dimension * y_dimension];

    std::vector<std::thread> thread_list;

    int segment_size =  y_dimension / thread_count ;
    int segment_size_adj=  y_dimension / thread_count;
    int segment_mod = y_dimension % thread_count;

    int segment_start =0;
    for( int current_thread = 0 ; current_thread < thread_count; ++current_thread){ 
        if(segment_mod > 0){
           
            segment_size_adj = segment_start + segment_size + 1;
            --segment_mod;
        }else{
            segment_size_adj = segment_start+ segment_size;
        }
        std::cout<<"Thread::"<< current_thread <<std::endl;
        std::cout<<"Start::"<< segment_start <<std::endl;
        std::cout<<"End::"<< segment_size_adj <<std::endl;
        
        thread_list.push_back( 
            std::thread(apply_mask,segment_start,
                        segment_size_adj, mask_size,
                        mask,
                        y_dimension,
                        x_dimension,
                        test_image,
                        fi)
            );
            segment_start = segment_size_adj;
    }
    //apply_mask(0,y_dimension , mask_size,mask,y_dimension, x_dimension,test_image, fi);

    for(int i = 0 ; i < thread_list.size(); ++i){
        thread_list[i].join();
    }
    std::cout<<"Saving..."<<std::endl;

    UWM::PGM().write(file_path_new,fi  , x_dimension, y_dimension);
    // Clean up
    for(int i = 0; i < mask_size; i++){
       delete mask[i];
    }
    delete mask;
    delete fi;
    return 0;
}