#include <iostream>
#include <string>
#include <vector>
#include "../headers/PGM.h"

int main(int argc, char *argv[]){
	int x=0, y=0,maxraw=0; 
	std::vector<unsigned char> image;
	if(argc < 3)
	{
		std::cout << "Usage:" << std::endl;
		std::cout << "\tPlease Provide" << std::endl;
		std::cout << "\t<file_path_original>" << std::endl;
		std::cout << "\t<file_path_new>" << std::endl;
		exit(-1);
	}
	std::string file_path_original = std::string(argv[1]);
	std::string file_path_new = std::string(argv[2]);
	uwm::PGM().read(file_path_original,image,x,y);
	uwm::PGM().write(file_path_new,image,x,y);
	return 0;
}