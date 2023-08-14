#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"

class Player: public Creature {
public:
    std::string race;
    Player(int max_health, int def, int atk, int hp, int gold, char theChar, std::string race);
};

#endif
