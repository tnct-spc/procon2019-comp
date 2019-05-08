#include "communication.h"


using namespace procon::communication;

BOOST_PYTHON_MODULE(communication){

    Py_Initialize();
    np::initialize();

    // procon::Fieldをpython側で用意せずに、numpy行列4つ(赤青のタイル状況、エージェント状況をboolにしたもの)を送る事にする
    // Fieldに関する他の関数も必要になったら適宜用意する
    bp::class_<Board>("Board", bp::init<std::string>())
            .def("getData", &Board::getData)
            .def("getScore", &Board::getScore)
            .def("addAct", &Board::addAgentAct)
            .def("getTurn", &Board::getTurn)
            .def("getAgentCount", &Board::getAgentCount)
            .def("getValidMoves", &Board::getValidMoves)
            .def("copy", &Board::copyBoard)
            .def("rev", &Board::reverseBoard)
            .def("isEnded", &Board::isEnded);
}
