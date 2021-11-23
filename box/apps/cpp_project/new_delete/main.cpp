#include <iostream>
#include <string>
#include <stdlib.h>


void test(){
    //char *pBuffer = (char*)malloc(12);
    char *pBuffer = new char[12];
    char *p = pBuffer;

    for(int i = 0; i < 10; i++){
        *p++ = 'b';
    }
    *p++ = '\0';

    std::cout << pBuffer << std::endl;

    delete[] pBuffer;
    //free pBuffer;
}



#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>


void test_simple(){
    char *pBuffer = new char[12];

    pBuffer[4] = '\0';


    std::cout << sizeof(pBuffer) << std::endl;
    std::cout << "addr of pBuffer: " << &pBuffer <<std::endl;

    printf("%#x\n",*pBuffer);

    delete[] pBuffer;
}

class resource{
    public:
        resource():
            str_c(new char[1])
        {
            std::cout << "默认构造函数被调用" << std::endl;
            *str_c = '\0';
        }

        resource(std::string str):
            str_c(new char[str.length()+1])
        {
            strcpy(str_c,str.c_str());
            std::cout << "拷贝构造函数被调用" << std::endl;
        }

        virtual ~resource(){
            std::cout << "析构函数被调用" << std::endl;
            delete[] str_c;
        }

        resource& operator=(const resource& other){
            std::cout << "复制构造函数被调用" << std::endl;
            if(this == &other){
                return *this;
            }
            delete[] str_c;
            str_c = new char[strlen(other.str_c)];
            strcpy(str_c,other.str_c);

            return *this;
        }

        resource& operator=(const char* c_str){
            std::cout << "复制构造函数被调用" << std::endl;
            delete[] str_c;
            str_c = new char[strlen(c_str)];
            strcpy(str_c,c_str);

            return *this;
        }


        const char* c_str(void){
            return str_c;
        }

        private:
            char* str_c;
};

void referece_test(void){

    int i = 12;
    int *p = &i;
    int &f = i;

    std::cout << f << std::endl;
    int* rr = std::move(p);
    if(p){
        std::cout << "p is not null" << std::endl;
        std::cout << *p << std::endl;
    }

    i = 14;
    std::cout << *rr << std::endl;
    std::cout << "------class test--------" << std::endl;

    std::string str("12");

    resource* r0 = new resource(str);
//    resource* r1 = std::move(r0);
    resource* r1 = new resource();

    *r1 = "12345";

    if(r0){
        std::cout << "r0 not null: " << r0->c_str() << std::endl;
    }

    if(r1){
        std::cout << "r1 not null: " << r1->c_str()<< std::endl;
    }

    delete r0;
    delete r1;

}

/*memory leak testing*/
void memory_leaking(){
    char *p = nullptr;
    int i = 0;
    while(1){
        usleep(200);
        p = new char[1024];
        snprintf(p,1024,"%6d\n",i);
        i++;
    }
}


int main()
{
    test();
    referece_test();
    return 0;
}


