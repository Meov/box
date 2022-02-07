#include "rsa_verify.hxx"
#include <unistd.h>
#include <sys/time.h>
#include <openssl/err.h>
#include <cstdio>

#define BUFFER_SIZE 512
const char hn[] = "SHA1";

RsaVerify::RsaVerify()
{
    OpenSSL_add_all_algorithms();
}

RsaVerify::~RsaVerify(){

}

std::string g_pubkey_string = "-----BEGIN PUBLIC KEY-----\n"\
                              "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAs6Zm2p3gUO0nEDm+wTlY\n"\
                              "DCIOLiMEeFWcc9KfQwqKDq9xI8oriKgW1HkCUvkFr0z93h3riRC4a3/dsL7dxKpi\n"\
                              "O3w4z2qnEYoktVsIeCH88lFXxe40GzhYhhOeDCPzBTiuC5qCmtD9dTbSUUn48K7x\n"\
                              "9VicXG9EbW+qmLaG58/bH7w7Lj9FdvzjzT2WOZrHVgCfXMjeREOdOG56hHWI5oTP\n"\
                              "jAgL9S2Q929mhHsevz+sDmQWnYovYpx7NI4hZh9uBcmMLGacFFvNCV7/y3RMVMSd\n"\
                              "O1WpNhIpoitd61w7c7NOeHXkvbBgfG1XPcuwlj79fciuz53sxkcjqbeLDsxM5COE\n"\
                              "IQIDAQAB\n"\
                              "-----END PUBLIC KEY-----\n";

EVP_PKEY * RsaVerify::load_pubkey(const std::string pubkey_file)
{
    EVP_PKEY *pubkey = NULL;
    BIO *bp = NULL;

    bp = BIO_new(BIO_s_mem());
    if (bp == NULL)
    {
        return NULL;
    }

    BIO_write(bp,pubkey_file.c_str(),pubkey_file.length());
    
    pubkey = PEM_read_bio_PUBKEY(bp, &pubkey, NULL, NULL);
    if (pubkey == NULL)
    {
        printf("Unable to load pubkey file %s\n",pubkey_file.c_str());
    }

    BIO_free(bp);
    return pubkey;
}

int RsaVerify::verify_it(const unsigned char *msg, size_t mlen, const unsigned char *sig, size_t slen, EVP_PKEY *pkey){

    /* Returned to caller */
    int result = -1;

    if (msg == NULL || mlen == 0 || sig == NULL || slen == 0 || pkey == NULL)
    {
        return -1;
    }

    EVP_MD_CTX *ctx = NULL;

    do
    {
        ctx = EVP_MD_CTX_create();
        if (ctx == NULL)
        {
            printf("EVP_MD_CTX_create failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }

        const EVP_MD *md = EVP_get_digestbyname("SHA1");
        if (md == NULL)
        {
            printf("EVP_get_digestbyname failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }

        int rc = EVP_DigestInit_ex(ctx, md, NULL);
        if (rc != 1)
        {
            printf("EVP_DigestInit_ex failed, error 0x%ld\n", ERR_get_error());
            break; /* failed */
        }

        rc = EVP_DigestVerifyInit(ctx, NULL, md, NULL, pkey);
        if (rc != 1)
        {
            printf("EVP_DigestVerifyInit failed, error 0x%ld\n", ERR_get_error());
            break; /* failed */
        }
        rc = EVP_DigestVerifyUpdate(ctx, msg, mlen);
        if (rc != 1)
        {
            printf("EVP_DigestVerifyUpdate failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }

        /* Clear any errors for the call below */
        ERR_clear_error();

        rc = EVP_DigestVerifyFinal(ctx, sig, slen);
        if (rc != 1)
        {
            printf("EVP_DigestVerifyFinal failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }

        result = 0;

    } while (0);

    if (ctx)
    {
        EVP_MD_CTX_destroy(ctx);
        ctx = NULL;
    }

    return result;
}

void RsaVerify::statistic(const timeval &start_time, const timeval &end_time, int result){
    uint64_t time_cost = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
    printf("RSA verifed cost time is：%ld us\n",time_cost);
    printf("rsa verified result: %d\n",result);
}

unsigned char* RsaVerify::read_data(const std::string& data_path, size_t& data_size){
    /* Read signature from file */
    FILE* fp = NULL;
    unsigned char *data_buffer = NULL;
    size_t bytes = 0;

    fp = fopen(data_path.c_str() , "r");
    if(fp == NULL){
        printf("%s open failed, error: %d\n",data_path.c_str(),errno);
        return NULL;
    }

    if(fsize(data_path.c_str(), bytes)<0){
        printf("get size of file: %s err\n",data_path.c_str());
        fclose(fp);
        return NULL;
    }

    data_buffer = (unsigned char *)malloc(bytes);
    if(data_buffer == NULL){
        printf("malloc failed\n");
        fclose(fp);
        return NULL;
    }

    data_size = fread(data_buffer, 1, bytes, fp);
    if(data_size != bytes){
        if(feof(fp)){
            printf("Error reading %s: unexpected end of file, error: %d\n",
                    data_path.c_str(),errno);
        }else if(ferror(fp)){
            printf("Error reading %s error: %d\n",data_path.c_str(),errno);
        }

        fclose(fp);
        free(data_buffer);
        return NULL;
    }

    fclose(fp);

    return data_buffer;
}

int RsaVerify::fsize(const char* filename, size_t& f_len){
    struct stat statbuf;
    if(stat(filename,&statbuf) == 0){
        f_len = statbuf.st_size;
        return 0;
    }

    return -1;
}

bool RsaVerify::verify_sig(const std::string& app_path,const std::string& app_sig_path){
    unsigned char *sig_buffer = NULL;
    size_t sig_size = 0;
    unsigned char *app_buffer = NULL;
    size_t app_size = 0;
    int result = -1;
    EVP_PKEY *rsa_pubkey = NULL;

    timeval start_time;
    timeval end_time;
    gettimeofday(&start_time, nullptr);

    do{
        sig_buffer = read_data(app_sig_path,sig_size);
        if(sig_buffer == NULL){
            printf("read app signature: %s failed\n",app_sig_path.c_str());
            break;
        }

        app_buffer = read_data(app_path,app_size);
        if(app_buffer == NULL){
            printf("read app: %s failed\n",app_path.c_str());
            break;
        }

        rsa_pubkey = load_pubkey(g_pubkey_string);
        if(rsa_pubkey == NULL){
            printf("rsa public key created failed\n");
            break;
        }

        result = verify_it(app_buffer,app_size,sig_buffer,sig_size,rsa_pubkey);

    }while(0);

    gettimeofday(&end_time, nullptr);
    statistic(start_time,end_time,result);

    /*free all buffer allocated*/
    if (rsa_pubkey)
    {
        EVP_PKEY_free(rsa_pubkey);
        rsa_pubkey = NULL;
    }

    if(sig_buffer){
        free(sig_buffer);
        sig_buffer = NULL;
    }

    if(app_buffer){
        free(app_buffer);
        app_buffer = NULL;
    }

    if(result == 0)
    {
        printf("App: %s Signature is valid!\n",app_path.c_str());
        return true;
    }
    else
    {
        printf("App: %s Signature is invalid!\n",app_path.c_str());
        return false;
    }
}

