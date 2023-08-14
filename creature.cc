#include "creature.h"
using namespace std;

void Creature::increaseAttribute(Attribute att, int num) {
    switch (att)
    {
    case HP:
        hp += num;
        if (hp > MAX_HEALTH) { hp = MAX_HEALTH; }
        break;
    case DEF:
        def += num;
        break;
    case GOLD:
        gold += num;
        break;
    case ATK:
        atk += num;
        break;
    case POISON:
        break;
    }
}

void Creature::decreaseAttribute(Attribute att, int num) {
    switch (att)
    {
    case HP:
        hp -= num;
        if (hp < 0) { hp = 0; }
        break;
    case DEF:
        def -= num;
        if (def < 0) { def = 0; }
        break;
    case GOLD:
        gold -= num;
        if (gold < 0) { gold = 0; }
        break;
    case ATK:
        atk -= num;
        if (atk < 0) { atk = 0; }
        break;
    case POISON:
        hp -= num;
        if (hp < 0) { hp = 1; }
        break;
    }
}

int Creature::getMax_health() const{
    return MAX_HEALTH;
}

int Creature::getHp() const{
    return hp;
}

int Creature::getAtk() const{
    return atk;
}

int Creature::getDef() const{
    return def;
}

int Creature::getGold() const{
    return gold;
}

char Creature::getChar() const{
    return theChar;
}

std::string Creature::getRace() const {
    return race;
}

Creature::~Creature(){}

bool Creature::available() {
    return false;
}


