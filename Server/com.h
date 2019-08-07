#include <string>
#include <boost/python.hpp>

class Com{
private:
    std::string ip,port,token;
    boost::python::object obj;
    boost::python::object matches, matchstatus, action, connection;
    void openFile();
public:
    Com(std::string, std::string,std::string);
    void setData(std::string, std::string,std::string);
    std::string getMatches();
    std::string getMatchStatus();
    std::string sendAction();
    std::string checkConnection();
};
