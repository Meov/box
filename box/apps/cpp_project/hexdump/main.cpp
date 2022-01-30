#include <iostream>
#define DEBUG

typedef struct databuff_t{
    void* buff;
    size_t len;
}msgbuff_t;

static void msg_xfer_debug(const msgbuff_t *data){
    int offset = 0;
    printf("---- data begin ----\n");
    for(long size = data->len; size > 0; size-=16) {
        long i;
    
        printf("%p ",((char*)(data->buff))+offset*16);

        for (i = 0; i < 16; i++) {
            if (i < size)
                printf("%02x ",((char*)(data->buff))[i+offset*16]);
            else
                printf("   ");
        }

        for (i = 0; i < 16; i++) {
            if (i < size)
                printf("%c", ((char*)(data->buff))[i+offset*16] >= 32 &&
                        ((char*)(data->buff))[i+offset*16] < 127 ? ((char*)(data->buff))[i+offset*16] : '.');
            else
                printf(" ");
        }
        offset++;
        printf("\n");
    }
    printf("---- data end ----\n");
}


int main(int argc, char* argv[]){
    std::cout << "data dump test..." << std::endl;
    const size_t data_len = 127;
    char data_trans[data_len] = {0};

    for(size_t i = 0; i < data_len; i++){
        data_trans[i] = i;
    }
#ifdef DEBUG 
    msgbuff_t data; 
    data.buff = data_trans;
    data.len =  data_len;
    msg_xfer_debug(&data);
#endif

}
