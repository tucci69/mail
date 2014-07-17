#ifndef __BACKTRACE_HPP__
#define __BACKTRACE_HPP__
#include <string>
#include <iostream>
#define DO_BACKTRACE()                                                                                  \
    do {                                                                                                \
        void *buffer[100];                                                                              \
        int nptrs = backtrace(buffer, 100);                                                             \
        std::cout << "backtrace() returned "<< nptrs << " addresses" << std::endl;                      \
        char** strings = backtrace_symbols(buffer, nptrs);                                              \
        for (int j = 0; j < nptrs; j++)                                                                 \
        {                                                                                               \
            std::string raw(strings[j]);                                                                \
            size_t pos = raw.find("_Z");                                                                \
            if(pos != std::string::npos)                                                                \
            {                                                                                           \
                size_t pos_plus = raw.find("+");                                                        \
                char* realname = abi::__cxa_demangle( raw.substr(pos, pos_plus-pos).c_str(), 0, 0, 0);  \
                std::cout << realname << std::endl;                                                     \
                free(realname);                                                                         \
            }                                                                                           \
        }                                                                                               \
    } while (0)
#endif
