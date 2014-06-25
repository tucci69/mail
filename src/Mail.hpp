#ifndef __MAIL_HPP__
#define __MAIL_HPP__

#include <asio.hpp>
#include <asio/ssl.hpp>
#include <iostream>
#include <ostream>
#include <list>
#include <string>
#include <memory>
#include <functional>

//#include "Resolver.hpp"
#include "logger.hpp"

using namespace std::placeholders;

double get_time();

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
    std::string                     m_host;
    std::string                     m_service;
    double                          m_start;

    asio::streambuf          m_request;
    asio::streambuf          m_response;

    void async_connect(error_code ec=error_code(),
            resolver::iterator iterator=resolver::iterator());
    void async_write();
    void async_read();
public:
    Mail(asio::io_service& ios,
            const std::string host,
            const std::string service) :
            m_ios(ios),
            m_resolver(ios),
            m_iterator(resolver::iterator()),
            m_context(asio::ssl::context::sslv23),
            m_socket(ios, m_context),
            m_host(host),
            m_service(service),
            m_start(0.0)
    {
       async_connect();
    }
    ~Mail(){ LOG_INFO("Destructor");}
    std::list<std::pair<std::string,double>>    m_time;
};

#endif // __MAIL_HPP__
