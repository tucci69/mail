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

namespace
{
    double  print_time(const string str, double x=0)
    {
        struct timeval tv={0};
        gettimeofday(&tv,NULL);
        double time = tv.tv_sec + tv.tv_usec / 1000000.0;
        cout << str << "\t\t\t" << fixed << time << endl;
        return time;
    }              
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
        print_time("*** Start                    ***");
        resolver::query q(m_host, m_service);
        m_resolver.async_resolve(q,bind(&Mail::async_connect, this, _1, _2));
        return;
    }

    for(tcp::resolver::iterator iter = m_iterator, end; iter != end; ++iter)
        std::cout << tcp::endpoint(*iter) << std::endl;

    print_time("*** DNSTime                  ***");

    m_context.set_default_verify_paths();
//    m_socket.set_verify_mode(asio::ssl::verify_peer);
    m_socket.set_verify_mode(asio::ssl::verify_none);
    m_socket.set_verify_callback(asio::ssl::rfc2818_verification(m_host)); 


    auto on_handshake = [this](const error_code& ec)
    {
        if(ec)
        {
            LOG_ERR(ec << " " << ec.message());
            return;
        }
        print_time("*** SSL connect time         ***");
        async_write();
    };

    auto on_connect = [this, on_handshake]
        (const error_code& ec, resolver::iterator iterator)
    {
        if(ec)
        {
            LOG_ERR(ec << " " << ec.message());
            return;
        }
       print_time("*** Connect                  ***");
       m_socket.async_handshake(asio::ssl::stream_base::client, on_handshake);
    };


    asio::async_connect(m_socket.lowest_layer(), iterator, on_connect);
            
    //    m_timer.expires_from_now(chrono::seconds(2),ec);
    //    m_timer.async_wait(bind(&Mail::async_connect, this, _1, m_iterator));
    //    m_timer.cancel();
}

void Mail::
async_write()
{
    ostream os(&m_request);
    string url = "/maps/place/London+SW20+0QW/@51.411424,-0.233385,3a,75y,76.82h,90t/data=!3m4!1e1!3m2!1s1DpRlfu2gkdAH8Ywjteo-g!2e0!4m2!3m1!1s0x487609207b9bcf7b:0xb9da41bb92006e88";
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
        
        print_time("*** Request sent time        ***");


        async_read();
    };

    print_time("*** Start request send time  ***");
    asio::async_write(m_socket, m_request, on_write);
}

void Mail::
async_read()
{
    auto on_read = [this](const error_code& ec, size_t bytes)
    {
        if(ec == asio::error::eof)
        {
            return;
        }
        else if (ec)
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

