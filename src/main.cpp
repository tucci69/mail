#include "Mail.hpp"
#include <memory>
#include <algorithm>
#include <iostream>

using namespace std;
int main()
{
    asio::io_service io_service; 

    Mail mail(io_service, "maps.google.com", "https");
    io_service.run();
    for_each(mail.m_time.begin(),mail.m_time.end(),
            [](pair<std::string,double>& elem)
            {
                cout << fixed << elem.first << " : " << elem.second << endl;
            }
    );
}
