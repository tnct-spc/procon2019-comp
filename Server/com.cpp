#include <fstream>
#include <streambuf>
#include <string>
#include <boost/python.hpp>
#include "com.h"

namespace ns = boost::python;
const char * filename = "com.py";


/*
int main(){
    auto main_ns = init();
    //openpy
    bp::exec(openfile().c_str(),main_ns);

    //runpy
    bp::object func = main_ns["func"];
    bp::object result = func();

    //output
    return bp::extract<string>(result);
}
*/

auto init(){
    Py_Initialize();
    auto main_mod = bp::import("___main__").attr("__dict__");
    return main_mod;
}

std::string openfile(char fname){
    std::ifstream ifs(filename);
    std::string script((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>(fname));
    return script;
}

std::string getData(){
    //init
    auto main_ns = init();
    
    //openpy
    bp::exec(openfile().c_str(),main_ns);

    //output
    /*
    std::cout << checkConnection(); << endl;
    
    */

    //output
    return checkConnection(main_ns);
}

std::string getMatches(auto * arg){
    bp::object obj = arg["getMatches"];
    bp::object ret = obj();
    return bp::extract<std::string>(ret);
}
std::string getMatchStatus(auto * arg){
    bp::object obj = arg["getMatchStatus"];
    bp::object ret = obj();
    return bp::extract<std::string>(ret);
}
std::string sendAction(auto * arg){
    bp::object obj = arg["sendAction"];
    bp::object ret = obj();
    return bp::extract<std::string>(ret);
}
std::string chechConnection(auto * arg){
    bp::object obj = arg["chechConnection"];
    bp::object ret = obj();
    return bp::extract<std::string>(ret);
}