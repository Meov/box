#ifndef __MYSTRING__
#define __MYSTRING__
#include <cstring>
#include <iostream>
typedef  unsigned long size_type;
class Mystring
{
	public:
		Mystring();
		~Mystring();
	
		Mystring(unsigned int count, char ch);
		Mystring(const char *s);
		//拷贝构造函数
		Mystring(const Mystring& s);
		//赋值构造函数
		Mystring& operator = (const Mystring& str);

		friend std::ostream& operator << (std::ostream &os,const Mystring& str);
		friend std::istream& operator >> (std::istream &is,const Mystring& str);

		bool operator == (const Mystring& str);
		const char& front() const;
		const char& back() const;
		Mystring& operator += (const Mystring& str);
		const char* c_str();
		char* get_str()const;
		size_type length();

	private:
		char *data_;
};

#endif

