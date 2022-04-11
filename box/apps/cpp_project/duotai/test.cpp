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
	cout<<"sizeof(B1) = "<<sizeof(B1)<<endl; //只有一个数据，其大小为 int 的大小。 sizeof(int) = 4 
	cout<<"sizeof(B2) = "<<sizeof(B2)<<endl; //包含虚表指针（64位机器是 8 字节，32位机器是4字节） 和 一个int型数据（4字节），在64位上， 4 + 8 = 12，若未声明内存按1字节对齐，则：该类大小为类中成员最大数据的倍数
    cout <<"sizeof(struct s) = " << sizeof(s) << endl;
    return 0;
}


