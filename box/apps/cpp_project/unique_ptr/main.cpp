#include <iostream>
#include <memory>

template<typename T>
class array_deleter {
    void operator()(T *p) {
        std::cout << "called array_deleter!" << std::endl;
        printf("addr of p [%p]\n",p);
//        delete[] p;
    }
};

class Test {
public:
    explicit Test(int a = 0) : a_(a), str_(new char[32]) { // explicit禁用隐式类型转换
        sprintf(str_, "%d", a_);
        printf("Test[%p].[%s] constructor\n", this, str_);
    }

    ~Test() {
        printf("~Test[%p].[%s] a: %d destructor\n", this, str_,a_);
        delete[] str_;
    }

public:
    int a_;
    char *str_;
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

int main(){
    test();

    {
        //std::unique_ptr<Test, array_deleter<Test>> t1(new Test[3]); //如果申请了数组类型个对象，在 unique_ptr析构时，需要手动释放 
        //std::unique_ptr<Test> t1(new Test[3]); //unique_ptr默认的析构函数 调用的是delete, 这样会内存泄漏！！！
        std::unique_ptr<Test, decltype(&test_deleter)> t2(new Test[4],test_deleter); //unique_ptr默认的析构函数 定义数组类型的class需要手动定义deletor
        
        std::cout << "ready to out of scope" << std::endl;
    }

/*
    Test* t1 = new Test(4);
    t1->a_ = 21;        
    std::cout << t1->a_ <<std::endl;
    delete t1;
    std::cout << "called delete" << std::endl;
    if(t1){

        std::cout<<"t1 after delete: " << t1->a_ << std::endl;
    }
    t1->a_ = 29;        
    std::cout << t1->a_ <<std::endl;
*/
}
