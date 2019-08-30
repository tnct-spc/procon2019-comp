#include <iostream>
#include "../Server/com.h"
#include "com.h"

void testServer(){
    //set data
    Com::setData("127.0.0.1", "8888", "procon30_example_token");
    //open file
    Com::openFile();
    //print data
    std::cout << Com::getMatches() << std::endl;
    std::cout << Com::getMatchStatus(1) << std::endl;
    std::cout << Com::sendAction(1, "") << std::endl;
    std::cout << Com::checkConnection() << std::endl;
    return ;
}
