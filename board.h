#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <chrono>
#include "dwarf.h"
#include "human.h"
#include "elves.h"
#include "orc.h"
#include "vampire.h"
#include "troll.h"
#include "werewolf.h"
#include "goblin.h"
#include "merchant.h"
#include "phoenix.h"
#include "ladder.h"
#include "compass.h"
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <memory>
#include "cell.h"
#include "potion.h"
#include "combat.h"
#include "gold.h"
#include "dragon.h"
#include "barriersuit.h"
using namespace std;

enum DragonItem {GOLDD, SUIT};
class Board {
    vector<vector<Cell>> boardTiles;
    vector<pair<int, int>> enemyCors;
    int numEnemies;
    int enemyAlive; //number of enemies alive
    int boardWidth = 79;
    int boardHeight = 25;
    pair <int, int> playerCoords;
    vector<pair<int, int>> goldCors;
    vector<pair<int, int>> dragonCors;
    vector<pair<int, int>> suitCors;
    Ladder theLadder = Ladder(make_pair(0, 0));
    bool compassSpawned;
    bool playerSpawned;
    bool ladderSpawned;
    bool hostile = false; //merchant not hostile to PC
    bool suitUp = false; //the player has not pickup the barrier suit;

    bool movePlayerN();
    bool movePlayerE();
    bool movePlayerS();
    bool movePlayerW();
    bool movePlayerNE();
    bool movePlayerSE();
    bool movePlayerSW();
    bool movePlayerNW();
    int grabPotionN();
    int grabPotionS();
    int grabPotionE();
    int grabPotionW();
    int grabPotionNW();
    int grabPotionSW();
    int grabPotionNE();
    int grabPotionSE();
    void moveSingleEnemy(pair<int, int> &i, int direction);
    bool moveCreatureN(pair<int, int> &Cor);
    bool moveCreatureS(pair<int, int> &Cor);
    bool moveCreatureE(pair<int, int> &Cor);
    bool moveCreatureW(pair<int, int> &Cor);
    void spawnCompass(pair<int, int>);
    void spawnEnemy(pair<int, int> Cor, char name);
    void spawnBarrierSuit(pair<int, int> cor_BS, Dragon* dragonPtr = nullptr); //BS short for Barrier Suit
    friend class Combat;
    void deleteEnemy(Cell &c);
    void spawnGold(pair<int, int> Cor, int amount, Dragon* dragonPtr = nullptr); ////amount could be 1 to 4; 1 and 2 being 
                                                                                //regular gold pile, 4 is merchant hoard, 6 is dragon hoard
    void checkAndGrabGold(Cell& cell); //check if its gold and pick gold if so and add to gold record of player
    //!!!!!!!!!!!!!!!!!!!! can also grab barrier suit, implement the effect of barrier suit


    Dragon* spawnDragon(pair<int, int> Cor);
    void spawnDragonItem(pair<int, int> cor_item, DragonItem item_name = GOLDD);

    public:
        void makeBoard(string file, int playerChoice, bool &BarrierSuitFlag, int floorNum);
        bool movePlayer(string theDir); // no,so,ea,we,ne,nw,se,sw
        void moveEnemy(); //COULD ADD TRAKCING PLAYER USING GREEDY 
        friend ostream &operator<<(ostream &out, Board b);
        pair<int, int> generateCoords();
        int getPlayerHp();
        int getPlayerAtk();
        int getPlayerDef();
        int getPlayerGold();
        string getPlayerRace();
        int generateRandomInt(int range); //generate random integer according to range, 
                                            // eg. range = 2, generate 0 or 1
        bool floorCleared();
        void dropCompass(pair<int, int> X);  // We can call this whenever a chosen enemy drops the compass
        void grabCompass(int X, int Y);
        int grabPotion(string theDir);
        int applyPotion(int n); // Returns an int from 0-5 depending on which potion was applied
                           // 0 - RH, 1 - BA, 2 - BD, 3 - PH, 4 - WA, 5 - WD
        // vector<Enemy*> getEnemies();
        
};

#endif
