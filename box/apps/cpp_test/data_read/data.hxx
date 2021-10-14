#ifndef __DATA_H
#define __DATA_H
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
typedef unsigned char bytes;

class Data{
	public:
		Data(const std::string filename);
		~Data();
		bytes* data_read(size_t& data_len);
	private:
		int fsize(const std::string filename, size_t &f_len);
		FILE* _fp;
		std::string _filename;
		bytes* _data_read;
};
#endif
