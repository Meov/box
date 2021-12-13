#ifndef __COMPLEX_H__
#define __COMPLEX_H__



#include <iostream>


class Complex{
    public:
        Complex(double r = 0, double i = 0)
            : re(r), im(i){
            }
        ~Complex(){
            std::cout << "destructor called" << std::endl;
        }

        /* 此处=后边的表达式用值传递时，会再次构造对象，出这个作用域时也会调用析构函数*/
        Complex& operator += (const Complex rhs){
            re = re + rhs.re;
            im = im + rhs.im;
            return *this;
        }
        /* 此处=后边的表达式用引用传递时，不会再次构造对象，出这个作用域时也不会调用析构函数
        Complex& operator += (const Complex rhs){
            re = re + rhs.re;
            im = im + rhs.im;
            return *this;
        }
        */

        double real() const {return re;}
        double imag() const {return im;}

    private:
        double re, im;
};


#endif
