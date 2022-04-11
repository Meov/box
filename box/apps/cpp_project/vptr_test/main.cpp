#include<iostream>
#include<string>
#include<typeinfo>
using namespace std;
#if 1
//#pragma pack(1) //内存按1字节对齐
class Base					//基类
{
public:
    ~Base(){
        cout << "~Base() called" << endl; 
    }
	virtual void TestFunc1()
	{
		cout << "Base::TestFunc1()" << endl;
	}
	virtual void TestFunc2()
	{
		cout << "Base::TestFunc2()" << endl;
	}
	virtual void TestFunc3()
	{
		cout << "Base::TestFunc3()" << endl;
	}

	int _b;
};
class Derived : public Base					//派生类
{
public:
    ~Derived(){
        cout << "~Derived() called" << endl;
    }
	virtual void TestFunc1()
	{
		cout << "Derived::TestFunc1()" << endl;
	}
	virtual void TestFunc2()
	{
		cout << "Derived::TestFunc2()" << endl;
	}

	virtual void TestFunc3()
	{
		cout << "Derived::TestFunc3()" << endl;
	}
	virtual void TestFunc4()
	{
		cout << "Derived::TestFunc4()" << endl;
	}
	int _d;
};


/*
 *  解析：(VTable)*(int64_t*)*(int64_t*)&a

    &a:得到(A*)指针
    (int64_t*)&a:将A*指针转化为int64_t*指针，指针指向a的内存空间的前八个字节
    *(int64_t*)&a:对指向前八个字节的指针解引用，发现里面的内容是一根指针，指向虚表，这个指针也就是vptr，
    
    即vptr == *(int64_t*)&a
    (int64_t*)*(int64_t*)&a:将vptr转化为(int64_t*)指针，也就是指向虚表的前八个字节的指针，也就是指向虚表第一个项(一个虚函数指针)的指针
    *(int64_t*)*(int64_t*)&a：对指向虚表第一个项的指针解引用，得到指针指向的内容，也就是第一个虚函数指针
    (VTable)*(int64_t*)*(int64_t*)&a:对虚函数指针转化为(VTable)函数指针，从而能够通过vtb进行调用
 *
 * */
typedef void(*PVFT)();				//声明函数指针，用来调用虚函数
typedef void (*FUNP2)(long f);
void PrintVFT(Base& b, const string& str)
{

    /*
    FUNP2 *fun2 = (FUNP2*)(((int64_t*)(int64_t*)&b)[0]);
    (*fun2)((int64_t)&b);
*/
    int i = 0;
	cout << str << endl;
    cout << "虚表内容 begin:" << endl;
    PVFT pVFT = (PVFT)(*((int64_t*)*((int64_t*)&b)));

	while(pVFT)
	{
        cout << "------->";
		pVFT();
        ++i;
        if (i == 1) continue;

		pVFT = (PVFT)*((int64_t*)*((int64_t*)&b)+i);
	}
    cout << "虚表内容vptr end" <<endl; 


/*
    pVFT();
    pVFT = (PVFT)*((int64_t*)*((int64_t*)&b)+2);
    pVFT();

    pVFT = (PVFT)*((int64_t*)*((int64_t*)&b)+3);
    pVFT();


    pVFT = (PVFT)*((int64_t*)*((int64_t*)&b)+4);
    pVFT();

    pVFT = (PVFT)*((int64_t*)*((int64_t*)&b)+6);
    if(pVFT){
         pVFT();
    }
  */
}
void TestVirtualFunc(Base& b)
{
	b.TestFunc1();
	b.TestFunc3();
	return;
}
int main()
{
//	Base b;
//	Derived d;
//    printA(b);
	// 打印基类与派生类的虚表
//	PrintVFT(b, "Base VFT:");
    Base* base = new Derived();

    PrintVFT(*base, "Derived VFT:");
#if 0
    // 传递Base类对象
	TestVirtualFunc(b);
	cout << endl;
	// 传递派生类对象
	TestVirtualFunc(d);
#endif
//    cout << "主动释放对象"<<endl;
//    delete base;
    return 0;
}

#else

class A {
public:
	virtual void f1() {
		cout << "A::f1()" << endl;
	}
	virtual void f2() {
		cout << "A::f2()" << endl;
	}
	void f3() {
		cout << "A::f2()" << endl;
	}
};

typedef void(*VTable)();

void printA(A& a) {
	VTable vtb = (VTable)*(int64_t*)*(int64_t*)&a;
	int  i = 0;
	while (vtb != NULL) {
		cout << "Function:" << ++i << endl;
		cout << "------->";
		vtb();
		vtb = (VTable)*((int64_t*)*(int64_t*)&a + i);
	}
}

int main(int argc,char* argv[])
{
	A a;
	printA(a);
}


#endif
