#include <string>
#include <boost/python.hpp>

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
