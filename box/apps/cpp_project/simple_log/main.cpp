#include <iostream>
#include "log.hpp"
#include <unistd.h>

int main(){
    std::cout << "log testing brgin..." << std::endl;
    log_debug("debug log");
    log_warn("warn log");
    log_info("info %s","dsfsdfsdf");
    log_error("error");

    log_level_set(LOG_ALL);
    while(1){
        usleep(100000);
        log_debug("debug log");
        log_warn("warn log");
        log_info("info %s","dsfsdfsdf");
        log_error("error");
    }

    return 0;
}
