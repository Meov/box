#include <iostream>
using namespace std;
class Person
{
	public:
		void Print(){
			cout<<"name:"<<_name<<endl;
			cout<<"age:"<<_age<<endl;
		}
	protected:
		string _name = "Romeo"; //姓名
		int _age = 18; //年龄
};
/*继承后父类的Person的成员（成员函数+变量）都会变成子类的一部分。这里
 * 体现出了Student和Teacher复用了Person的成员。*/
class Student: public Person
{
	protected:
		int _stuid; //学号
};

class Teacher:public Person
{
	protected:
		int _jobid; //工号
};

int main(){
	Student student;
	student.Print();
}
