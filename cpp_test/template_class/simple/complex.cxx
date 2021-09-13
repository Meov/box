#include "complex.h"

int main(int argc, char *argv[]){

	Complex<int> c1(1,2);
	std::cout << c1.imag() << std::endl;

	Complex<double> c2(1.0,2.3);
	std::cout << c2.imag() << std::endl;

	return 0;
}
