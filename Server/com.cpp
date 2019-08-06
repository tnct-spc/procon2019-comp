#include <fstream>
#include <streambuf>
#include <string>
#include <boost/python.hpp>
#include "com.h"

const char * filename = "com.py";
namespace ns = boost::python;

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

auto init(){
    Py_Initialize();
    auto main_mod = bp::import("___main__").attr("__dict__");
    return main_mod;
}

std::string openfile(){
    std::ifstream ifs(filename);
    std::string script((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    return script;
}

string getData(){
    //init
    auto main_ns = init();
    
    //openpy
    bp::exec(openfile().c_str(),main_ns);

    //runpy
    bp::object func = main_ns["getData"];
    bp::object result = func();

    //output
    return bp::extract<string>(result);
}