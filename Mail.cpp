#include <asio.hpp>

#include <iostream>
#include <ostream>
#include <list>
#include <string>
#include <memory>

#include "Mail.hpp"
#include "logger.hpp"
#include "utils.hpp"

using namespace std;

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
        resolver::query q(m_host, m_service);
        m_resolver.async_resolve(q,bind(&Mail::async_connect, this, _1, _2));
        return;
    }
    for(tcp::resolver::iterator iter = m_iterator, end; iter != end; ++iter)
        std::cout << tcp::endpoint(*iter) << std::endl;

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
    os << "GET /mail/feed/atom HTTP/1.0\r\n";
//    os << "User-Agent: curl/7.36.0\r\n";
    os << "Host: " <<  m_host << "\r\n";
//    os << "Authorization: Basic aWdvci50YW5AZ21haWwuY29tOklsMHYzTDBwM3o=\r\n";
    os << "Cookie: gmail=bPvHAxzu2kYh-BUWeEhU9Q\r\n";
    os << "Accept: */*\r\n";
    os << "Connection: close\r\n\r\n";

    auto on_write = [this](const error_code& ec, size_t bytes)
    {
        if(ec)
        {
            LOG_ERR(ec << " " << ec.message());
            return;
        }
        async_read();
    };
    asio::async_write(m_socket, m_request, on_write);
}

void Mail::
async_read()
{
    auto on_read = [this](const asio::error_code& ec, size_t bytes)
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

