#include "a.hpp"
void print_namespace::aprintf(int m, int n){
    std::cout << "m: "<< m << " n: " << n << std::endl;
}

print_namespace::class_a::class_a(int a):_data(a)
{
    std::cout << "a_class constructor called" << std::endl;
}

print_namespace::class_a::~class_a()
{
    std::cout << "a_class dstructor called" << std::endl;
}

void print_namespace::class_a::class_a_test(void){
    std::cout <<__FILE__ <<":"<< __func__ << ":" << __LINE__<<" data :"<< _data << std::endl;
}
