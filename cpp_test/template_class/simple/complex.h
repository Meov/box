#ifndef __COMPLEX_H__
#define __COMPLEX_H__



#include <iostream>

template<typename T>

class Complex{
	public:
		Complex(T r = 0, T i = 0)
			: re(r), im(i){
		}		
	Complex& operator += (const Complex&);
	T real() const {return re;}
	T imag() const {return im;}

	private:
		T re, im;
};


#endif
