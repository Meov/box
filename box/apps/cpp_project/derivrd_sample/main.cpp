#include <iostream>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

//effective c++ 中条款37 不要重写继承而来的非虚函数的参数值

//参数是静态绑定的，

class Base{
    public:
        virtual void draw(int color = 1){
            std::cout << "Base color: " << color  << std::endl;
            do_draw(color);
        }
    private:
        virtual void  do_draw(int color) const = 0;
};

class Son : public Base{
    public:
/* //此处不能重新定义缺省值
        void draw(int color = 2){
            std::cout << "Base color: " << color  << std::endl;
            do_draw(color);
        }
*/
    private:
        void  do_draw(int color) const {
            std::cout << "Son really draw color: " << color  << std::endl;
        }
};


int main()
{
    Son s;   //派生
    s.draw();

    Base* b = &s; //多态
    b->draw();
    return 0;
}


