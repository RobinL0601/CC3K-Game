#ifndef VAMPIRE_H
#define VAMPIRE_H

#include "enemy.h"

class Vampire : public Enemy {
    public:
    Vampire(): 
        Enemy{50, 50, 25, 25, 1, 'V'} {}
};

#endif
