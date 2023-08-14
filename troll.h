#ifndef TROLL_H
#define TROLL_H

#include "enemy.h"

class Troll : public Enemy {
    public:
    Troll(): 
        Enemy{120, 120, 25, 15, 1, 'T'} {}
};

#endif
