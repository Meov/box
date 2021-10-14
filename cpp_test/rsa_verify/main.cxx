#include "rsa_verify.hxx"
#include <iostream>


int main(int argc, char*argv[]){
	bool result = false;
	RsaVerify rsa_verify;
	if(argc < 3){
		std::cout << "Usage: ./verify DATA DATA.sig" << std::endl;
		exit(0);
	}

	std::string data_path(argv[1]);
	std::string data_sig_path(argv[2]);

	result = rsa_verify.verify_sig(data_path,data_sig_path);
	if(result)
	{
		std::cout << "verified successfully!" << std::endl;
	}
	else
	{
		std::cout << "verified failed!" << std::endl;
	}

	return 0; 
}
