#include <iostream>
#include <ostream>
#include <iomanip>
#include <list>
#include <string>
#include <memory>

#include "Mail.hpp"
#include "logger.hpp"
#include "utils.hpp"

using namespace std;

    double get_time()
    {
        struct timeval tv={0};
        gettimeofday(&tv,NULL);
        return tv.tv_sec + tv.tv_usec / 1000000.0;
    }              



void Mail::
async_connect(error_code ec, resolver::iterator iterator)
{
    if(ec)
    {
        LOG_ERR(ec << " " << ec.message());
        return;
    }

    m_iterator=iterator;

    if( m_iterator == resolver::iterator() )
    {
        m_time.push_back(make_pair("*** Start                    ***",get_time()));
        resolver::query q(m_host, m_service);
        m_resolver.async_resolve(q,bind(&Mail::async_connect, this, _1, _2));
        return;
    }

//    for(tcp::resolver::iterator iter = m_iterator, end; iter != end; ++iter)
//        std::cout << tcp::endpoint(*iter) << std::endl;

    m_time.push_back(make_pair("*** DNSTime                  ***",get_time()));

    auto on_connect = [this]
        (const error_code& ec, resolver::iterator iterator)
    {
        if(ec)
        {
            LOG_ERR(ec << " " << ec.message());
            return;
        }
        m_time.push_back(make_pair("*** Connect                  ***",get_time()));
        async_write();
    };


    asio::async_connect(m_socket.lowest_layer(), iterator, on_connect);
            
}

void Mail::
async_write()
{
    ostream os(&m_request);
    string url = "/";
    os << "GET " << url << " HTTP/1.1\r\n";
    os << "User-Agent: curl/7.36.0\r\n";
    os << "Host: " <<  m_host << "\r\n";
    os << "Accept: */*\r\n";
    os << "Connection: close\r\n\r\n";

    auto on_write = [this](const error_code& ec, size_t bytes)
    {
        if(ec)
        {
            LOG_ERR(ec << " " << ec.message());
            return;
        }
        
        m_time.push_back(make_pair("*** Request sent time        ***",get_time()));


        async_read();
    };

    m_time.push_back(make_pair("*** Start request send time  ***",get_time()));
    asio::async_write(m_socket, m_request, on_write);
}

void Mail::
async_read()
{
    auto on_read = [this](const error_code& ec, size_t bytes)
    {
        m_time.push_back(make_pair("*** Read                     ***",get_time()));
        if (ec)
        {
             LOG_ERR(ec << " " << ec.message());
           return;
        }
        cout << &m_response;
        async_read();
    };

    asio::async_read(m_socket,
            m_response, asio::transfer_at_least(1),on_read);
}

