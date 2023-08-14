#ifndef GOBLIN_H
#define GOBLIN_H
#include "enemy.h"

class Goblin : public Enemy {
    public:
    Goblin(): 
        Enemy{70, 70, 5, 10, 1, 'N'} {}
};

#endif
