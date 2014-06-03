#include "Mail.hpp"
#include <memory>

int main()
{
    asio::io_service io_service; 

    Mail mail(io_service, "mail.google.com", "https");
    io_service.run();
}
