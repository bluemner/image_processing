#ifndef _BETACORE_GOLDEN_SECTION_SEARCH_H_
#define _BETACORE_GOLDEN_SECTION_SEARCH_H_
#include <iostream>
#include <cmath>
namespace betacore{
	template<typename T>
	class GoldenSectionSearch{

		public:
			 GoldenSectionSearch(){

			}
			 ~GoldenSectionSearch(){

			}
			T f(T x){
				return 0.5 - x * exp((x * -x));
			}
		    void search(T a, T b, T tolerance){
			T t = (sqrt(5) - 1) /2;
			T x1 = a +  (1 - t) * ( b - a );
			T f1 = f(x1);
			T x2 = a + t * (b - a);		
			T f2 = f(x2);
			size_t precision = 5;
			size_t width = 10 + precision;
			char fill =' ';
			std::cout<<" "<<std::fixed<<std::setprecision(precision)<<std::setfill(fill)<<std::setw(width)<<std::endl;
			std::cout<<"" <<std::endl;
			while((b-a) > tolerance) {
				if( f1 > f2){
					a = x1;
					x1 = x2;
					f1 = f2;
					x2 =  a + t * (b-a);
					f2 = f(x2);
				}else{
					b = x2;
					x2 = x1;
					f2 = f1; 
					x1 = a + (1 - t ) * (b-a);
					f1 = f(x1);
				}
				std::cout<< "\tx1:" << x1 << "\tf1:" << f1  << "\tx2:" << x2 << "\tf2:" << f2  << std::endl;
			}
		}
	};

}
#endif