#ifndef COMPASS_H
#define COMPASS_H
#include "creature.h"

class Compass : public Creature {
    public:
    Compass(): 
        Creature{0, 0, 0, 0, 0, 'C'} {}
};

#endif
