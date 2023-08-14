#ifndef POTION_H
#define POTION_H
#include "creature.h"

class Potion: public Creature {
    int potType;
    public:
        Potion(int x): Creature(0, 0, 0, 0, 0, 'P'), potType(x) {}
        int getPotType() override { return potType; }
};

#endif
