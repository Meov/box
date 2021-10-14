#ifndef __COMPLEX_H__
#define __COMPLEX_H__



#include <iostream>


class Complex{
	public:
		Complex(double r = 0, double i = 0)
			: re(r), im(i){
		}		
	Complex& operator += (const Complex&);
	double real() const {return re;}
	double imag() const {return im;}

	private:
		double re, im;
};


#endif
