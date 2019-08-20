#include <iostream>
#include "../Server/com.h"
#include "com.h"

void testServer(){
    //set data
    Com::setData("127.0.0.1", "8888", "procon30_example_token");
    //open file
    Com::openfile();
    //print data
    std::cout << Com::getMatches() << endl;
    std::cout << Com::getMatchStatus(1) << endl;
    std::cout << Com::sendAction(1, ""); << endl;
    std::cout << Com::checkConnection(); << endl;
    //change ip/port/token
    return ;
}
