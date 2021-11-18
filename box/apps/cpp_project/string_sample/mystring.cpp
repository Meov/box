#include "mystring.hxx"
#include <iostream>

Mystring::~Mystring(){
    delete[] data_;
}

Mystring::Mystring():
    data_(new char[1])
{
    *data_ = '\0';
}

Mystring::Mystring(const char *s):
    data_(new char[strlen(s)+1])
{
    strcpy(data_,s);
}

Mystring::Mystring(unsigned int count, char ch):
    data_(new char[(count+1)])
{
    memset(data_,ch,count);
    data_[count] = '\0';
}

/*
 *	
 *	should overide copy ctor and assign function if there are 
 *	any pointer or reference variable
 *
 * */
Mystring::Mystring(const Mystring& str):
    data_(new char[strlen(str.data_)+1])
{
    strcpy(data_,str.data_);
}

Mystring& Mystring::operator=(const Mystring& str){

    if(this == &str){
        return *this;
    }

    delete[] data_;
    data_ = new char[strlen(str.data_)+1];
    strcpy(data_,str.data_);

    return *this;
}

Mystring& Mystring::operator=(const char* c_str){
    delete[] data_;
    data_ = new char[strlen(c_str)+1];
    strcpy(data_,c_str);

    return *this;
}

/** ctors end ** */

char* Mystring::get_str() const{
    return data_;
}

const char& Mystring::front() const{
    return data_[0];
}

std::ostream& operator << (std::ostream &os,const Mystring& str){
    os << str.data_;
    return os;
}

std::istream& operator >> (std::istream &is,const Mystring& str){
    is >> str.data_;
    return is;
}

bool Mystring::operator == (const Mystring& str){
    if(strlen(data_) != strlen(str.data_))
        return false;
    if(strcmp(data_,str.data_)){
        return false;
    }
    return true;
};

Mystring& Mystring::operator+(const Mystring& str){

    char* buffer = new char[strlen(str.data_)+ strlen(data_) +1];
    strcpy(buffer,data_);
    strcat(buffer,str.data_);
    Mystring string_tmp(buffer);
    return string_tmp;
}

Mystring& Mystring::append(const Mystring& str){

    char* buffer = new char[strlen(str.data_)+ strlen(data_) +1];
    strcpy(buffer,data_);
    strcat(buffer,str.data_);
    Mystring string_tmp(buffer);

    delete[] buffer;
    *this = string_tmp;
    return *this;
}

const char* Mystring::c_str(){
    return data_;
};

size_type Mystring::length(){
    return strlen(data_);
};
