
#ifndef CREATURE_H
#define CREATURE_H

#include "cell.h"
#include <string>

enum Attribute {DEF, ATK, HP, GOLD, POISON};

class Creature {
protected:
    const int MAX_HEALTH;
    int hp;
    int atk;
    int def;
    int gold;
    char theChar;
    std::string race;
public:
    Creature(int max_health, int hp, int atk, int def, int gold, char theChar): 
        MAX_HEALTH{max_health}, hp{hp}, atk{atk}, def{def}, gold{gold}, theChar{theChar}, race{""} {}
    Creature(int max_health, int hp, int atk, int def, int gold, char theChar, std::string theRace): 
        MAX_HEALTH{max_health}, hp{hp}, atk{atk}, def{def}, gold{gold}, theChar{theChar}, race{theRace} {}
    int getMax_health() const;
    int getHp() const;
    int getAtk() const;
    int getDef() const;
    int getGold() const;
    char getChar() const;
    std::string getRace() const;
    void setRace(std::string theRace);
    void increaseAttribute(Attribute att, int num);
    void decreaseAttribute(Attribute att, int num);
    virtual int getPotType() { return -1; }
    virtual bool available();
    virtual void makeAvailable() {};
    //virtual destructor
    virtual ~Creature();
};


#endif

