#include <fstream>
#include <streambuf>
#include <string>
#include <boost/python.hpp>
#include "com.h"


namespace bp = boost::python;
const char * filename = "com.py";


Com::Com(std::string ipAddress, std::string portNumber, std::string postToken){
    this->setData(ipAddress, portNumber, postToken);
}


void Com::setData(std::string ipAddress, std::string portNumber, std::string postToken){
    this->ip = ipAddress;
    this->port = portNumber;
    this->token = postToken;
    Py_Initialize();
    this->openFile();
}


void Com::openFile(){
    //namespace
    auto main_ns = bp::import("___main__").attr("__dict__");
    //open file
    std::ifstream ifs(filename);
    //read script
    std::string script((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    bp::exec(script.c_str(),main_ns);
    //obj
    this->matches = main_ns["getMatches"];
    this->matchstatus = main_ns["getMatchStatus"];
    this->action = main_ns["sendAction"];
    this->connection = main_ns["checkConnection"];
}


std::string Com::getMatches(){
    //get data
    auto ret = this->matches(this->ip, this->port, this->token);
    //parse pyobj
    return bp::extract<std::string>(ret);
}

std::string Com::getMatchStatus(){
    //get data
    auto ret = this->matchstatus(this->ip, this->port, this->token);
    //parse pyobj
    return bp::extract<std::string>(ret);
}

std::string Com::sendAction(){
    //get data
    auto ret = this->action(this->ip, this->port, this->token);
    //parse pyobj
    return bp::extract<std::string>(ret);
}

std::string Com::checkConnection(){
    //get data
    auto ret = this->connection(this->ip, this->port, this->token);
    //parse pyobj
    return bp::extract<std::string>(ret);
}





/* ----- usage example -----
int main(){
    //instance
    Com ins("127.0.0.1", "8888", "procon30_example_token");
    //pointer
    Com * ins_p;
    ins_p = new Com("127.0.0.1", "8888", "procon30_example_token");
    //print data
    std::cout << ins.getMatches() << endl;
    //change ip/port/token
    ins.setData("192.168.0.0", "404", "mokemoke");
    return 0;
}
//*/
