#ifndef BARRIERSUIT_H
#define BARRIERSUIT_H
#include "enemy.h"

class Barriersuit : public Creature {
    Enemy* enemyPtr;
    public:
    Barriersuit(Enemy* guardian): 
        Creature{0, 0, 0, 0, 0, 'B'}, enemyPtr{guardian} {}
    ~Barriersuit(){}
    bool available() override; //whether the barrier suit could be picked up
    void makeAvailable() override;
};

#endif
