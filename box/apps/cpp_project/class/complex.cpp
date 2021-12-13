#include "complex.h"
#include <unistd.h>

int main(int argc, char *argv[]){

	Complex c2(1.0,2.3);
	Complex c1(9.0,4.7);

    c1 += c2;

	std::cout << c1.imag() << std::endl;
	int i = 0;
	std::cout << "sizeof class Complex: " << sizeof(Complex) <<std::endl;
	std::cout << "sizeof double: " << sizeof(double) <<std::endl;
	std::cout << "sizeof int: " << sizeof(int) <<std::endl;
	std::cout << "sizeof long long: " << sizeof(long long) <<std::endl;

#if 0
	double startTime,stopTime,durationTimeTime;
	double start, stop, durationTime;
	startTime=time(NULL);
	start = clock();

	while(i<5){
		sleep(1);
		Complex* complex  = new Complex(3,2);
		std::cout << complex->imag() << std::endl;
		i++;
	}

	stopTime=time(NULL);
    durationTimeTime = (double)difftime(stopTime,startTime);

	stop = clock();
	durationTime = ((double)(stop - start));
	std::cout << "总耗时：" << durationTime  << std::endl;

	std::cout << "耗时(time): " << durationTimeTime << " s" << std::endl;
#endif	
	return 0;
}
