#include "combat.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

// constructor
Combat::Combat(Board* b): board{b} {}

// Damage(Defender) = ceiling((100/(100 + Def(Defender))) ∗ Atk(Attacker))

int Combat::damage(int atk,int def) {
    float num;
    float x = 100;
    float atkF = atk;
    float defF = def;
    return num = ceil((x/(x + defF)) * atkF);
}

string Combat::enemyType(char theChar) {
    if (theChar == 'V') {
        return "Vampire";
    } else if (theChar == 'W') {
        return "Werewolf";
    } else if (theChar == 'N') {
        return "Goblin";
    } else if (theChar == 'T') {
        return "Troll";
    } else if (theChar == 'X') {
        return "Phoenix";
    } else if (theChar == 'M') {
        return "Merchant";
    } else {
        return "Dragon";
    }
}

bool Combat::isEnemy(char theChar) {
    if (theChar == 'V' || theChar == 'W' ||
        theChar == 'N' || theChar == 'X' ||
        theChar == 'T' || theChar == 'M' || theChar == 'D') {
            return true;
        } else {
            return false;
        }
}

// attackEnemy returns true if the enemy is still alive after one round of attacking, false o/w
bool Combat::attackEnemy(Creature* player,pair<int, int> cor) {
    Creature*& enemy = board->boardTiles[cor.second][cor.first].ctrptr;
    Cell& cell = board->boardTiles[cor.second][cor.first];
    if (enemy == nullptr) {
        return true;
    }
    if (isEnemy(enemy->getChar())) { // basic enemy types
        int damageE = damage(player->getAtk(),enemy->getDef());
            string race;
            race = enemyType(enemy->getChar());
            if (enemy->getChar() == 'M') {
                if (board->hostile == false) {
                    board->hostile = true;
                }
            }
        if (damageE >= enemy->getHp()) {
            enemy->decreaseAttribute(HP,enemy->getHp()); // enemy died
            cout<< "You killed a " << race << "." << endl;
            if (enemy->getChar() != 'D' && enemy->getChar() != 'M') {
                player->increaseAttribute(GOLD,1);
                board->deleteEnemy(cell);
            } else if (enemy->getChar() == 'M') { // Merchant
                board->deleteEnemy(cell);
                board->spawnGold(board->generateCoords(),4);
            } else {//dragon
                if (board->boardTiles[cor.second-1][cor.first].ctrptr && (board->boardTiles[cor.second-1][cor.first].ctrptr->getChar() == 'G' ||
                        board->boardTiles[cor.second-1][cor.first].ctrptr->getChar() == 'B')) {
                            board->boardTiles[cor.second-1][cor.first].ctrptr->makeAvailable();
                        }
                if (board->boardTiles[cor.second+1][cor.first].ctrptr && (board->boardTiles[cor.second+1][cor.first].ctrptr->getChar() == 'G' ||
                        board->boardTiles[cor.second+1][cor.first].ctrptr->getChar() == 'B')) {
                            board->boardTiles[cor.second+1][cor.first].ctrptr->makeAvailable();
                        }
                if (board->boardTiles[cor.second][cor.first+1].ctrptr && (board->boardTiles[cor.second][cor.first+1].ctrptr->getChar() == 'G' ||
                        board->boardTiles[cor.second][cor.first+1].ctrptr->getChar() == 'B')) {
                            board->boardTiles[cor.second][cor.first+1].ctrptr->makeAvailable();
                        }
                if (board->boardTiles[cor.second][cor.first-1].ctrptr && (board->boardTiles[cor.second][cor.first-1].ctrptr->getChar() == 'G' ||
                        board->boardTiles[cor.second][cor.first-1].ctrptr->getChar() == 'B')) {
                            board->boardTiles[cor.second][cor.first-1].ctrptr->makeAvailable();
                        }
                board->deleteEnemy(cell);
                // dragon hoard available
            }
            //drop Compass
            if (board->compassSpawned == false) {
                if (board->enemyAlive == 1) {
                    board->dropCompass(cor);
                } else {
                    int randInt = board->generateRandomInt(board->numEnemies);
                    if (randInt == 0) {
                        board->dropCompass(cor);
                    }
                }
            }
            board->enemyAlive --;
            return false;
        } else {
            enemy->decreaseAttribute(HP,damageE);
            cout<< "You dealt: "<< damageE << " damage to the " << race << "!" << endl;
            return true;
        } 
    }
    return true;
}

// attack returns true if the player is still alive after one round of attacking, false o/w
bool Combat::attack() {
    const int x = board->playerCoords.first;
    const int y = board->playerCoords.second;
    Creature*& player = board->boardTiles[y][x].ctrptr;
    string theDir; // attack direction 
    cin >> theDir;
    if (theDir == "no") {
        pair<int, int> cor;
        cor.first = x;
        cor.second = y - 1; 
        if (!attackEnemy(player,cor)) {
                // enemy died
            }
    } else if (theDir == "so") {
        pair<int, int> cor;
        cor.first = x;
        cor.second = y + 1; 
        if (!attackEnemy(player,cor)) {
                // enemy died
            }
    } else if (theDir == "ea") {
        pair<int, int> cor;
        cor.first = x + 1;
        cor.second = y; 
        if (!attackEnemy(player,cor)) {
                // enemy died
            }
    } else if (theDir == "we") {
        pair<int, int> cor;
        cor.first = x -1;
        cor.second = y; 
        if (!attackEnemy(player,cor)) {
                // enemy died
            }
    } else if (theDir == "ne") {
        pair<int, int> cor;
        cor.first = x + 1;
        cor.second = y - 1; 
        if (!attackEnemy(player,cor)) {
                // enemy died
            }
    } else if (theDir == "nw") {
        pair<int, int> cor;
        cor.first = x - 1;
        cor.second = y - 1; 
        if (!attackEnemy(player,cor)) {
                // enemy died
            }
    } else if (theDir == "se") {
        pair<int, int> cor;
        cor.first = x + 1;
        cor.second = y + 1; 
        if (!attackEnemy(player,cor)) {
                // enemy died
            }
    } else if (theDir == "sw") {
        pair<int, int> cor;
        cor.first = x - 1;
        cor.second = y + 1; 
        if (!attackEnemy(player,cor)) {
                // enemy died
            }
    }
    return attackPlayer();
}

bool Combat::attackPlayer () {
    const int x = board->playerCoords.first;
    const int y = board->playerCoords.second;
    int totaldamageP = 0; //total damage for player
    Creature*& player = board->boardTiles[y][x].ctrptr;
        // enemies attack player
    for (int i = x - 1; i < x + 2; i++) {
        for (int j = y - 1; j < y + 2; j++) {
            int damageP = 0;
            if (i == x && j == y) {
                continue; // enemy is player
            }
            Creature*& enemy = board->boardTiles[j][i].ctrptr;
            if (enemy == nullptr) {
                continue; // enemy not exist
            }
            if (isEnemy(enemy->getChar())) { // basic enemy types
                damageP = damage(enemy->getAtk(),player->getDef());
                if (enemy->getChar() == 'M' && !board->hostile) {
                    damageP = 0;
                } else {
                    // attack missed
                    int random = board->generateRandomInt(2);
                    string race;
                    race = enemyType(enemy->getChar());
                    if (random == 1) {
                        damageP = 0;
                        cout<< "You were attacked by a "<< race << " but they missed their attack!" << endl;
                        sleep(1);
                    } else {
                        cout<< "You took damage: " << damageP << " from " << race << "!" << endl;
                        sleep(1);
                    }
                }
            }
            if (board->suitUp) {
                damageP = damageP/2 ;
                sleep(1);
            }
            totaldamageP += damageP;
            damageP = 0;
        }
    }
    if (totaldamageP >= player->getHp()) {
        player->decreaseAttribute(HP,player->getHp());
        return false; //player died;
    } else {
        player->decreaseAttribute(HP,totaldamageP);
        return true; // player alive
    }
}
