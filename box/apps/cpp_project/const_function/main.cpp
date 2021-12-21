#include<iostream>
 
using namespace std;
 
class Test
{
public:
    Test(){}
    Test(int _m):_cm(_m){}

    int get_cm() const  //类中的const成员函数，表明该函数不会修改对成员数据
    {
       st_num = 18; //正确 可以修改静态数据成员，因为该成员不属于任何对象！
       //return _cm++; // 错误！在const成员函数中，不能修改类中的对象， ‘Test::_cm’ in read-only object
       return _cm; //正确
    }
 
    //const static int st_num = 12; //仅仅只有const 类型的静态成员可以在类的声明中初始化
    static int st_num; 
private:
    int _cm;
};
 
int Test::st_num = 12; 
 
void Cmf(const Test& _tt)
{
    cout<<_tt.get_cm() << endl;
    cout<<Test::st_num<< endl;
}
 
int main(void)
{
    Test t(8);
    Cmf(t);
    return 0;
}
