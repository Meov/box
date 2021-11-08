#include <iostream>
using namespace std;

int main(){
	
	std::cout << "将字符数组变为string类型时，当其中提前出现了 '\0' 便表名字符串结束" << std::endl;
	std::cout << "如：char c[5] = {'a','b',\\'\0\\','c','d'}; string s = string(c),输出如下： "<<std::endl;

	char c[5] = {'a','b','\0','c','d'};
	string s = string(c);

	printf("size of s: %ld\n",sizeof(s.c_str()));
	printf("s: %s\n",s.c_str());

	cout << s << endl;


	std::cout << "初始化数组：int a[10] = {5}" << std::endl;
	int a[10] = {5};
	for(int i = 0; i < 10; i++){
		printf("a[%d]: %d\n",i,a[i]);
	}
	std::cout << "可以发现只有第一个被初始化成 5 其余的为 0" << std::endl;

	return 0;
}

