#include "Mail.hpp"
#include <memory>
#include <algorithm>
#include <iostream>

using namespace std;
int main()
{
    asio::io_service io_service; 

    Mail mail(io_service, "dealerportal.autotrader.co.uk", "https");
    io_service.run();
    double tm_start = mail.m_time.begin()->second;
    for_each(mail.m_time.begin(),mail.m_time.end(),
            [&tm_start](pair<std::string,double>& elem)
            {
                cout << fixed << elem.first << " : " << elem.second <<
                    " : " << (int)round(100 * (elem.second - tm_start)) << endl;
            }
    );
}
