#ifndef GOLD_H
#define GOLD_H
#include "enemy.h"

class Gold : public Creature {
    public:
    Enemy* enemyPtr;
    Gold(int amount, Enemy* guardian): 
        Creature{0, 0, 0, 0, amount, 'G'}, enemyPtr{guardian} {}
    ~Gold(){}
    bool available() override; //whether the gold pile could be picked up
    void makeAvailable() override;
};

#endif
