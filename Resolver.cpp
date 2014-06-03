#include "Resolver.hpp" 
#include "logger.hpp"



Resolver::
Resolver(asio::io_service& io_service,
            const std::string host,
            const std::string port)
    :
        resolver_(io_service),
        query_(host,port),
        iterator_(iterator())
{
}

void Resolver::
resolve()
{
    int h=0;
    using namespace std::placeholders;
    resolver_.async_resolve(query_,
           bind(&Resolver::on_resolve,this,_1,_2,h)); 
}

void Resolver::
on_resolve(error_code ec, iterator iterator,int h)
{
    if(ec)
    {
        LOG_ERR("Error: " << ec << " " << ec.message());
        return;
    }
    iterator_ = iterator;
    for(resolver::iterator iter = iterator, end; iter != end; ++iter)
        LOG_INFO(tcp::endpoint(*iter));
}

