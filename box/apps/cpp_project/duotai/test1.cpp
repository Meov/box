#include<iostream>
using namespace std;
#pragma pack(1) //内存按1字节对齐

class B1
{
	public:
		void func1()
		{}
		int _b;
};
class B2
{
	public:
		virtual void func()
		{}
		int _b;
};

struct s{
    int a;  //4
    int *b; //8
};


int main()
{
	cout<<"sizeof(B1) = "<<sizeof(B1)<<endl; 
	cout<<"sizeof(B2) = "<<sizeof(B2)<<endl; 
    B2* b = new B2();
    cout << b << endl;
    cout << &(b->_b) << endl;

    delete b;




    return 0;
}


