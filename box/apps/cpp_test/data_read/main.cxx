#include "data.hxx"

bytes* q = nullptr;

void test(bytes* &p){
	q = (bytes*)malloc(10);
	p = q;
	printf("addr of q: %p\n",q);
}

void test_free()
{
	free(q);
	q = nullptr;
}
int main(){ 
#if 0
	bytes *p = nullptr;
	test(p);
	printf("addr of q: %p\n",q);
	printf("addr of p: %p\n",p);
	test_free();
	printf("addr of q: %p\n",q);
	printf("addr of p: %p\n",p);


#else
	size_t data_len = 0;
	bytes* buffer = nullptr;
	Data data("1.txt");	
	Data data_1("2.txt");	
	printf("addr of buffer: %p\n",buffer);
	printf("addr of buffer's addr: %p\n",&buffer);

	buffer = data.data_read(data_len);
	if(buffer == nullptr){
		std::cout << "read data err" << std::endl;
	}
	std::cout << data_len << std::endl;
	printf("addr of buffer: %p\n",buffer);

#endif
	return 0;
}
