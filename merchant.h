#ifndef MERCHANT_H
#define MERCHANT_H
#include "enemy.h"

class Merchant : public Enemy {
    // bool aggressive;
    public:
    Merchant(): 
        Enemy{30, 30, 70, 5, 1, 'M'} /*aggressive{false}*/ {}
    // bool getAttitude();
    // void alterAttitude();
};

#endif
