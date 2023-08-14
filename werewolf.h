#ifndef WEREWOLF_H
#define WEREWOLF_H
#include "enemy.h"

class Werewolf : public Enemy {
    public:
    Werewolf(): 
        Enemy{120, 120, 30, 5, 1, 'W'} {}
};

#endif
