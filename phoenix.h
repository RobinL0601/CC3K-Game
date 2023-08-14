#ifndef PHOENIX_H
#define PHOENIX_H
#include "enemy.h"

class Phoenix : public Enemy {
    public:
    Phoenix(): 
        Enemy{50, 50, 35, 20, 1, 'X'} {}
};

#endif
