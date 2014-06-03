#ifndef __MAIL_HPP__
#define __MAIL_HPP__

#include <config.h>
#include <asio.hpp>
#include <asio/ssl.hpp>
#include <asio/coroutine.hpp>

#include <iostream>
#include <ostream>
#include <list>
#include <string>
#include <memory>
#include <functional>

#include "Resolver.hpp"
#include "logger.hpp"

using namespace std::placeholders;

class Mail
{   
    typedef asio::ip::tcp tcp;
    typedef asio::error_code error_code;
    typedef tcp::resolver resolver;
    typedef asio::ssl::stream<tcp::socket> ssl_socket;

    asio::io_service&               m_ios;
    resolver                        m_resolver;
    resolver::iterator              m_iterator;
    asio::ssl::context              m_context;
    asio::ssl::stream<tcp::socket>  m_socket;
    asio::steady_timer              m_timer;
    std::string                     m_host;
    std::string                     m_service;

    asio::streambuf                 m_request;
    asio::streambuf                 m_response;

    void async_connect(error_code ec=error_code(),
            resolver::iterator iterator=resolver::iterator());
    void async_write();
    void async_read();
public:
    Mail(const Mail&) = delete;
    Mail& operator=(const Mail&) = delete;
    Mail(asio::io_service& ios,
            const std::string host, const std::string service) :
            m_ios(ios),
            m_resolver(ios),
            m_iterator(resolver::iterator()),
            m_context(asio::ssl::context::sslv23),
            m_socket(ios, m_context),
            m_timer(ios),
            m_host(host),
            m_service(service)
    {

       async_connect();
    }
    ~Mail(){ LOG_INFO("Destructor");}
};

#endif // __MAIL_HPP__
