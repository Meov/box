#include <iostream>
using namespace std;


#if 0 
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

#if 0
   cout << "a: "<< a << endl;
   cout << "b: "<< b << endl;
   swapr(a,b);
   cout << "a: "<< a << endl;
   cout << "b: "<< b << endl;
#endif

   return 0;
}
#endif



int main(){
	int i = 0;
	int &r = i;
	int *p = &i;
	r = 4;
    *p = 5;
	cout << "addr of r: " << &r << " addr of i: " << &i << endl;
	cout << "addr of p: " << &p << " addr of i: " << &i << endl;
	return 0;
}

