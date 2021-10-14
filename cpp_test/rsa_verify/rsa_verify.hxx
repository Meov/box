#ifndef RSA_VERIFY_H
#define RSA_VERIFY_H

#include <stdio.h>
#include <stdlib.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <iostream>
#include <sys/time.h>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

class RsaVerify{
    public:
        RsaVerify();
        ~RsaVerify();
        bool verify_sig(const std::string& app_path,const std::string& app_sig_path);
    private:
        /*or the fixed public key in code*/
        void statistic(const timeval &start_time, const timeval &end_time, int result);
        EVP_PKEY *load_pubkey(const std::string pubkey_file);
        unsigned char* read_data(const std::string& data_path, size_t& data_size);
        int fsize(const char* filename, size_t& f_len);
        int verify_it(const unsigned char *msg, size_t mlen, const unsigned char *sig, size_t slen, EVP_PKEY *pkey);
};
#endif
