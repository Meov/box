#include <iostream>
#include <memory>

template<typename T>
class array_deleter {
    void operator()(T *p) {
        std::cout << "called array_deleter!" << std::endl;
        printf("addr of p [%p]\n",p);
        delete[] p;
    }
};

class Test {
    public:
        explicit Test(int a = 0) 
            : a_(a), 
            str_(new char[32]),
            str_g("")
    { // explicit禁用隐式类型转换
        sprintf(str_, "%d", a_);
        printf("Test[%p].[%s] constructor\n", this, str_);
        printf("str_g: %s\n",str_g.c_str());
    }


    void set_str(const std::string& str){
        str_g = str;
        printf("after set str, str_g: %s\n",str_g.c_str());
    }

    void show_str() const{
        printf("show str: %s\n",str_g.c_str());
    }

    ~Test() {
        printf("~Test[%p].[%s] a: %d destructor\n", this, str_,a_);
        delete[] str_;
    }

public:
    int a_;
    char *str_;
    std::string str_g;
};


void read_file(){
    //std::unique_ptr<FILE, int(*)(FILE*)> fp(fopen("1.txt","r"), fclose);
    std::unique_ptr<FILE, decltype(&fclose)> fp(fopen("1.txt","r"),&fclose); //decltype 会自动推导参数类型· 它的作用是选择并返回操作数的数据类型

    char read_buf[2];
    int cnt = fread(read_buf,sizeof(read_buf),1,fp.get());
    std::cout << cnt << std::endl;
    for(auto i:read_buf){
        std::cout << i << std::endl;
    }
}


void test_deleter(Test* t) {
    printf("addr of p [%p]\n",t);
    delete[] t;
};

int test(void)
{
   FILE * pFile;
   char buffer [100];

   pFile = fopen ("myfile.txt" , "r");
   if (pFile == NULL) perror ("Error opening file");
   else
   {
     while ( ! feof (pFile) )
     {
       if ( fgets (buffer , 100 , pFile) == NULL ) break;
       fputs (buffer , stdout);
     }
     fclose (pFile);
   }
   return 0;
}

using  test_shared_ptr = std::shared_ptr<Test>;
int main(){
    {
#if 0
        //std::unique_ptr<Test> t1(new Test[3]); //unique_ptr默认的析构函数 调用的是delete, 这样会内存泄漏！！！
        //std::unique_ptr<Test, array_deleter<Test>> t1(new Test[3]); //如果申请了数组类型个对象，在 unique_ptr析构时，需要手动释放 可以用这种方式指定 
        std::unique_ptr<Test[], decltype(&test_deleter)> t2(new Test[4],test_deleter); //定义数组类型的class需要手动定义deletor 也可以用这种方式指定
        t2[1].set_str("3241234");       
        printf("t2[0]: %p\n",&t2[0]);
        printf("t2[1]: %p\n",&t2);
        printf("t2[2]: %p\n",&t2[2]);
        printf("t2[3]: %p\n",&t2[3]);

        std::cout << "ready to out of scope" << std::endl;
#endif
        test_shared_ptr test_ptr(new Test);
        test_ptr->set_str("3241234");
    }
}
