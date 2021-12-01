#include <iostream>
#include <map>
#include <string.h>
#include <vector>
using namespace std;

 
void vector_test()
{
    vector<int>obj;//创建一个向量存储容器 int
    for(int i=0;i<10;i++) // push_back(elem)在数组最后添加数据 
    {
        obj.push_back(i);
        cout<<obj[i]<<",";    
    }
 
    for(int i=0;i<5;i++)//去掉数组最后一个数据 
    {
        obj.pop_back();
    }
 
    cout<<"\n"<<endl;
 
    for(int i=0;i<obj.size();i++)//size()容器中实际数据个数 
    {
        cout<<obj[i]<<",";
    }
}

void map_test(){
    // declaration of map container
    map<char, int> mymap;
    mymap['a'] = 1;
    mymap['b'] = 2;
#if 1
    mymap['c'] = 3;
#endif
    std::map<char, int>::iterator it;

    std::cout << "map size: "<<mymap.size() << std::endl;
   // for (it = mymap.begin(); it != mymap.end(); it++){ //这是错的，迭代器不能这样删除！！！
    for (it = mymap.begin(); it != mymap.end();){
        cout << it->first << " = "<< it->second << '\n';
        mymap.erase(it++);
//or use like:
//		it = mymap.erase(it);
//		mymap.erase(it); 是错的！当前的迭代器被删除掉了，无法遍历到下一个迭代器
		cout << "map size update: "<<mymap.size() << std::endl;
    }

    cout << "after map size update: "<<mymap.size() << std::endl;
    mymap.clear();
}

int main()
{
    vector_test();
    map_test();
    return 0;
}
