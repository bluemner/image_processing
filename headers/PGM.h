#ifndef _UWM_PGM_H_
#define _UWM_PGM_H_

#include <vector>
#include <iostream>
#include <string>
namespace uwm{
	/*
		Encapsulated read method for simple c++ use
	*/
	class PGM{
	public:
		
		/*
			file_path: absolute file path
			&image unsigned char vector where the image will be stored
		*/
		static void read(
			const std::string file_path,
			std::vector<unsigned char> &vector_image,
			int &x_dimension,
			int &y_dimension);
		static void write(
			const std::string file_path,
			std::vector<unsigned char> &vector_image,
			const int &x_dimension,
			const int &y_dimension);
		
	
	};
}
#endif
