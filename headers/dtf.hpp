#include <iostream>
#include <cmath>

namespace betacore{

	template <typename T>
	class DFT{
		private:
		constexpr double pi() { return std::atan(1)*4; }
		public:
		template <size_t N>
		static T calculate(T (&X)[N]){
			T a;
			T w = exp( -2* pi() / N );
			for(size_t k=0; k < N; k++){
				X[k] =0;
				a =1;
				for(size_t j=0; j < N; j++){
					X[k] = X[k] +a *x[j];
					a = a* pow(w,k);
				}
			}
		}
	}
}