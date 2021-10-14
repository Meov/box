#include <iostream>
 
using namespace std;


void swapr(int &a, int& b){
	int temp;
	temp = b;
	b = a;
	a= temp;
}

int main ()
{
   // 声明简单的变量
   double d;
   int    i;
 
   // 声明引用变量
   int&    r = i;
   double& s = d;
	
   i = 123;
   d = 1.23;
	

   cout << "Value of i reference : " << r  << endl;
   cout << "Value of d reference : " << s  << endl;
  

   cout << sizeof(int) <<endl;
   cout << sizeof(double) <<endl;
   cout << "addr of r: " << &r << " addr of i: " << &i << endl;
   cout << "addr of s: " << &s << " addr of d: " << &d << endl;

	
   int a = 3;
   int b = 4;


   cout << "a: "<< a << endl;
   cout << "b: "<< b << endl;
   swapr(a,b);
   cout << "a: "<< a << endl;
   cout << "b: "<< b << endl;


   return 0;
}
