#include "com.h"

namespace bp = boost::python;

#define FILENAME "./../../procon2019-comp/Server/com.py" //:thiking_face:
//#define FILENAME "./com.py" //something does not work ?
//#define FILENAME "/home/okdwtr/Documents/procon2019-comp/Server/com.py"   //this works good but...

void Com::setData(std::string inpAddress, std::string postToken){
    address = inpAddress;
    token = postToken;
    openFile();
}

void Com::openFile(){
    //namespace
    auto main_ns = bp::import("__main__").attr("__dict__");
    //open file
    std::ifstream ifs(FILENAME);
    //read script
    std::string script((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    bp::exec(script.c_str(),main_ns);
    //obj
    matches = main_ns["getMatches"];
    matchstatus = main_ns["getMatchStatus"];
    action = main_ns["sendAction"];
    connection = main_ns["checkConnection"];
}


std::string Com::getMatches(){
    //get data
    auto ret = matches(address, token);
    //parse pyobj
    std::string str = bp::extract<std::string>(ret);
    return parseJson(str);
}

std::string Com::getMatchStatus(int id){
    //get data
    auto ret = matchstatus(address, token, id);
    //parse pyobj
    std::string str = bp::extract<std::string>(ret);
    return parseJson(str);
}

std::string Com::sendAction(int id, std::string arg){
    //get data
    auto ret = action(address, token, id, arg);
    //parse pyobj
    std::string str = bp::extract<std::string>(ret);
    return parseJson(str);
}

std::string Com::checkConnection(){
    //get data
    auto ret = connection(address, token);
    //parse pyobj
    std::string str = bp::extract<std::string>(ret);
    return parseJson(str);
}

std::string Com::parseJson(std::string& str){
    if(str.back() == '$'){
        std::cout << "----error----" << std::endl;
        std::cout << str.substr(0, str.size() - 1) << std::endl;
        std::cout << "-------------" << std::endl;
        return "error";
    }
    return str;
}




/* ----- usage example -----
int main(){
    //set data
    Com::setData("127.0.0.1", "8888", "procon30_example_token");
        //other way to set data(not recommended)
        Com::address = "192.168.0.0";
        Com::port = "404";
        Com::token = "not found";
    //open file
    Com::openfile();
    //print data
    std::cout << Com::getMatches() << endl;
    std::cout << Com::getMatches(1) << endl;
    std::cout << Com::sendAction(1, ""); << endl;
    std::cout << Com::checkConnection(); << endl;
    return 0;
}
*/
