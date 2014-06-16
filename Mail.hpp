#ifndef __MAIL_HPP__
#define __MAIL_HPP__

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <iostream>
#include <ostream>
#include <list>
#include <string>
#include <memory>
#include <functional>

//#include "Resolver.hpp"
#include "logger.hpp"

using namespace std::placeholders;

class Mail
{   
    typedef boost::asio::ip::tcp tcp;
    typedef boost::system::error_code error_code;
    typedef tcp::resolver resolver;
    typedef boost::asio::ssl::stream<tcp::socket> ssl_socket;

    boost::asio::io_service&        m_ios;
    resolver                        m_resolver;
    resolver::iterator              m_iterator;
    boost::asio::ssl::context       m_context;
    boost::asio::ssl::stream<tcp::socket>  m_socket;
    std::string                     m_host;
    std::string                     m_service;
    double                          m_start;

    boost::asio::streambuf          m_request;
    boost::asio::streambuf          m_response;

    void async_connect(error_code ec=error_code(),
            resolver::iterator iterator=resolver::iterator());
    void async_write();
    void async_read();
public:
    Mail(boost::asio::io_service& ios,
            const std::string host, const std::string service) :
            m_ios(ios),
            m_resolver(ios),
            m_iterator(resolver::iterator()),
            m_context(boost::asio::ssl::context::sslv23),
            m_socket(ios, m_context),
            m_host(host),
            m_service(service),
            m_start
    {

       async_connect();
    }
    ~Mail(){ LOG_INFO("Destructor");}
};

#endif // __MAIL_HPP__
