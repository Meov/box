#include "mystring.hxx"
#include <iostream>

int main(){
	std::cout<<"call normal ctor"<<std::endl;
	Mystring str("Hello");
	std::cout << str.get_str() << std::endl;

	std::cout<<"call normal numbered charactors ctor"<<std::endl;
	Mystring str_1(9,'c');
	std::cout << str_1.c_str() << std::endl;

	std::cout<<"call copy ctor"<<std::endl;
	Mystring str_3(str);
	std::cout << str_3.c_str() << std::endl;

	std::cout<<"call assigned function"<<std::endl;
	Mystring str_2 = str_1;
	std::cout << str_2.get_str() << std::endl;
	std::cout << str_2.length() << std::endl;

	/*operator override*/
	std::cout << str_2 <<std::endl;
	std::cout << str_2.c_str() << std::endl;
	if(str_1 == str_2){
		std::cout << "str_1 == str_2" << std::endl;
	}

	return 0;
}
