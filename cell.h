#ifndef CELL_H
#define CELL_H
#include <utility>

class Creature;
struct Cell {
    std::pair<int, int> coords;
    char tileVal;
    Creature* ctrptr = nullptr;
    Cell(char c, int x, int y, Creature *ctr): coords{x, y}, tileVal{c}, ctrptr{ctr} {}
};

#endif

