#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;
class Person
{
	public:
		Person(const char* name = "Romeo")
			:_name(name){
				cout << "Person()" << endl;
			}

		Person(const Person& p)
			:_name(p._name){
				cout << "Person(const Person& p)" << endl;
			}

		Person& operator=(const Person& p){
			cout << "Person& operator=(const Person& p)" << endl;
			if (this != &p){
				_name = p._name;
			}
			return *this;
		}

		~Person(){
			cout << "~Person()" << endl;
		}
	protected:
		string _name; //姓名
};

class Student :public Person
{
	public:
		Student(const char* name, int num):Person(name) //调用父类的构造函数完成从父类继承来的成员的初始化
			 , _num(num)
	{
		cout << "Student()" << endl;
	}

		Student(const Student& s)
			:Person(s) //调用父类的拷贝构造函数，其中也为“切片”操作，因为父类的拷贝构造函数的参数为引用
			 , _num(s._num)
	{
		cout << "Student(const Student& s)" << endl;
	}

		Student& operator=(const Student& s){
			cout << "Student& operator=(const Student& s)" << endl;
			if (this != &s){
				Person::operator=(s);  //调用基类的
				_num = s._num;
			}
			return *this;
		}

		~Student(){
			//此处不需要显示调用父类的析构函数，编译器会自动调用
			cout << "~Student()" << endl;
		}
	protected:
		int _num; //学号
};

void Test()
{
	Student s1("jack", 18);
	Student s2(s1);
	Student s3("rose", 17);
	s1 = s3;
}

int main()
{
	Test();
	system("pause");
	return 0;
}

