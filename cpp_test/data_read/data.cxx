#include "data.hxx"
Data::Data(const std::string filename):
_fp(nullptr),_filename(filename),_data_read(nullptr)
{
	//donothing
}

Data::~Data(){
	std::cout << "dtor called" << std::endl;
	if(_fp){
		std::cout << "clsoe fp" << std::endl;
		fclose(_fp);
		if(ferror(_fp)){
        	std::cout <<_filename<<" closed failed, error: "<<errno<<std::endl;
    	}
		_fp = nullptr;
	}

	if(_data_read){
		free(_data_read);
		std::cout << "free data read buffer" << std::endl;
		_data_read = nullptr;
	}
}

bytes* Data::data_read(size_t& data_len){
	size_t f_len = 0;
	size_t data_size = 0;

	_fp = fopen(_filename.c_str(),"r");
	if(_fp == nullptr){
		std::cout <<_filename<<" opened failed, error: "<<errno<<std::endl;
		return nullptr;
	}

	if(fsize(_filename,f_len)){
		std::cout <<_filename<<" get size error"<<std::endl;
		return  nullptr;
	}

	_data_read = (bytes* )malloc(f_len);
	if(_data_read == nullptr){
		std::cout <<"malloc failed, errno:"<<errno<<std::endl;
        return nullptr;
	}

	data_size = fread(_data_read, 1, f_len, _fp);
	if(data_size != f_len){
		if(feof(_fp)){
			std::cout << "Error reading "<<_filename<<" unexpected end of file, error: "<<
				errno << std::endl;
		}else if(ferror(_fp)){
			std::cout << "Error reading"<<_filename<<" error: "<<errno <<std::endl;
		}
		return nullptr;
	}
	
	data_len = data_size;
	return _data_read;
}

int Data::fsize(const std::string filename, size_t &f_len){
    struct stat statbuf;
    if(stat(filename.c_str(),&statbuf) == 0){
        f_len = statbuf.st_size;
        return 0;
    }

    return -1;
}
