#ifndef STATES_H
#define STATES_H

#include <vector>

namespace procon{

const std::vector<int> dx = {-1, -1, -1, 0, 0, 1, 1, 1, 0};
const std::vector<int> dy = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

class Point
{
public:
    Point();
};

struct MoveState{
};


}

#endif // STATES_H
