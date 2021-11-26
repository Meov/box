#include <iostream>
#include "a.hpp"
#include "b.hpp"

#define USE_NAMESPACE_PRINT using namespace print_namespace;

//USE_NAMESPACE_PRINT

int main(){
    
    std::cout << "excute function by using namespace: " << std::endl;
    print_namespace::aprintf(1,3);
    print_namespace::bprintf(1,3);

    std::cout << "run class sample by using namespace: " << std::endl;
    print_namespace::class_a classa(3);
    classa.class_a_test();
}
