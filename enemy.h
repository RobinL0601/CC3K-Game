#ifndef ENEMY_H
#define ENEMY_H

#include "creature.h"

// abstract Enemy class
class Enemy : public Creature {
    public:
        Enemy(int max_health, int hp, int atk, int def, int gold, char theChar): 
        Creature{max_health, hp, atk, def, gold, theChar} {}
    // Enemy(int atk, int ) : Creature{}
};

#endif
