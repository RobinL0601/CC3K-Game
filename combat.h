#ifndef COMBAT_H
#define COMBAT_H

#include <string>
#include "board.h"
#include "creature.h"
#include "gold.h"

class Board;
class Combat {
    Board* board;
    // enemyType returns the race of the enemy
    string enemyType(char theChar);
    // damage calculates the damage
    int damage(int atk,int def);
    // isEnemy returns true if it is an enemy
    bool isEnemy(char theChar);
    public:
    // Combat constructor
    Combat(Board* b);
    // attack returns true if the player is still alive after one round of attacking, false o/w
    bool attack();
    bool attackEnemy(Creature* player,pair<int, int> cor);
    bool attackPlayer();
};

#endif
