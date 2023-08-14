#ifndef LADDER_H
#define LADDER_H
using namespace std;

class Ladder {
    pair<int, int> ladderCoords;
    bool visible;

    public:
        Ladder(pair<int, int> X): ladderCoords{X}, visible{false} {}
        int getXCoord() { return ladderCoords.first; }
        int getYCoord() { return ladderCoords.second; }
        bool isVisible() { return visible; }
        void makeVisible() { visible = true; }
};
#endif
