#include<iostream> 
using namespace std; 

class Apple 
{ 
    public: 
        Apple(){
            cout << "constructor called " << endl;
        }

        ~Apple(){
            cout << "destructor called " << endl;
        }
        // static member function 
        static void printMsg() 
        {
            cout<<"Welcome to Apple!" << endl; 
        }

        void print_info(){
            cout << "This is none static function" << endl;
        }
}; 

// main function 
int main() 
{ 
    {
        // invoking a static member function 
        Apple::printMsg(); 
        Apple p;

        static Apple a;
        a.print_info();
        a.printMsg();
    }
    cout << "out of {}" << endl;

}

//ref:  https://github.com/Light-City/CPlusPlusThings/tree/master/basic_content/static
