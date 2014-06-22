#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__
#include <iostream>


#define LOG_INFO( msg )             \
    do { std::cout <<               \
        "File: " << __FILE__ << \
        ", func: " << __func__ <<   \
        ", line: " << __LINE__ <<   \
        ", " << msg << std::endl;   \
    } while(0)

#define LOG_ERR( msg ) LOG_INFO( msg )
#endif
