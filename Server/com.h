#ifndef COM_H
#define COM_H

#pragma push_macro("slots")
#undef slots
#include <boost/python.hpp>
#pragma pop_macro("slots")

namespace Com{
    std::string ip,port,token;
    boost::python::object obj;
    boost::python::object matches, matchstatus, action, connection;
    void openFile();
    void setData(std::string, std::string,std::string);
    std::string getMatches();
    std::string getMatchStatus(int);
    std::string sendAction(int, std::string);
    std::string checkConnection();
};

#endif // COM_H
