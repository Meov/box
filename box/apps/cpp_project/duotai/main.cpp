#include <iostream> 
using namespace std;
 
class Shape {
   protected:
      int width, height;
   public:
      Shape( int a=0, int b=0)
      {
         width = a;
         height = b;
      }
      int area()
      {
         cout << "Base class Parent class area :" <<endl;
         return 0;
      }

      virtual ~Shape(){
          cout << "Base destructor called" << endl;
      }
};


class Rectangle: public Shape{
   public:
      int num;
      Rectangle( int a=0, int b=0):Shape(a, b) { num = 3;  }
      int area ()
      { 
         cout << "Rectangle Rectangle class area :" << (width * height)  <<endl;
         return (width * height); 
      }
      ~Rectangle(){
        cout << "Rectangle destructor called" << endl;
      }
};


class Triangle: public Shape{
   public:
      Triangle( int a=0, int b=0):Shape(a, b) { }
      int area ()
      { 
         cout << "Triangle Triangle class area :" << (width * height / 2) <<endl;
         return (width * height / 2); 
      }
      ~Triangle(){
        cout << "Triangle destructor called" << endl;
      }
};


// 程序的主函数
int main( )
{
   //Shape *shape;
   
     Shape *shape = new Rectangle(10,7);
     //Shape *shape = new Shape(1,3);
     shape->area();
     delete shape;
/*
     Rectangle *r = new Rectangle(1,2);
     r->area();
     cout << r->num << endl;
*/
#if 0
   Rectangle rec(10,7);
   Triangle  tri(10,5);

   // 存储矩形的地址
   shape = &rec;
   // 调用矩形的求面积函数 area
   shape->area();
 
   // 存储三角形的地址
   shape = &tri;
   // 调用三角形的求面积函数 area
   shape->area();
#endif

//   delete r;
   return 0;
}
