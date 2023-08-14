#ifndef DRAGON_H
#define DRAGON_H
#include "enemy.h"

class Dragon : public Enemy {
    public:
    Dragon(): 
        Enemy{150, 150, 20, 20, 0, 'D'} {}
};

#endif
