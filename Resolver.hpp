#ifndef __RESOLVER_HPP__
#define __RESOLVER_HPP__
#include <asio.hpp>
#include <string>

class Resolver
{
    typedef asio::ip::tcp tcp;
    typedef asio::error_code error_code;
    typedef tcp::resolver resolver;
    typedef resolver::iterator iterator;

    resolver resolver_;
    resolver::query query_;
    iterator iterator_;

    void on_resolve(error_code, iterator,int);
public:
    Resolver(asio::io_service&,
            const std::string,   // host
            const std::string);  // port 
    void resolve();
    iterator getIterator(){ return iterator_; }
};

#endif
