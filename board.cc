#include <fstream>

#include "board.h"
#include <math.h>
using namespace std;

void Board::makeBoard(string file, int playerChoice, bool &barrierSuitFlag, int floorNum) {
    // carry score between floors
    int playerGoldAmt = 0;
    if (floorNum > 1) {
        playerGoldAmt = getPlayerGold();
    }
    
    // clear old board
    boardTiles.clear();
    enemyCors.clear();
    compassSpawned = false;
    playerSpawned = false;
    ladderSpawned = false;

    // initialize new board using board.txt
    ifstream infile{file};
    char theChar;
    boardTiles.resize(boardHeight);
    for (int i = 0; i < boardHeight; i++) {
        for (int j = 0; j < boardWidth; j++) {
            infile.get(theChar);
            Creature *tmpCtr = nullptr;
            if (theChar == '\n') {
                infile.get(theChar);
            }
            if ('0' <= theChar && theChar <= '5') {
                tmpCtr = new Potion(int(theChar - '0'));
                theChar = '.'; //WD
            } else if (theChar == '6') {
                tmpCtr = new Gold(1,nullptr);
                theChar = '.'; //normal gold pile
            } else if (theChar == '7') {
                tmpCtr = new Gold(2,nullptr);
                theChar = '.'; //small hoard
            } else if (theChar == '8') {
                tmpCtr = new Gold(4,nullptr);
                theChar = '.'; //merchant hoard
            } else if (theChar == '9') {
                tmpCtr = new Gold(6,nullptr);
                theChar = '.'; //dragon hoard
            } else if (theChar == 'V') {
                tmpCtr = new Vampire{};
                enemyCors.push_back(make_pair(j, i));
                theChar = '.';
            } else if (theChar == 'W') {
                tmpCtr = new Werewolf{};
                enemyCors.push_back(make_pair(j, i));
                theChar = '.';
            } else if (theChar == 'N') {
                tmpCtr = new Goblin{};
                enemyCors.push_back(make_pair(j, i));
                theChar = '.';
            } else if (theChar == 'M') {
                tmpCtr = new Merchant{};
                enemyCors.push_back(make_pair(j, i));
                theChar = '.';
            } else if (theChar == 'X') {
                tmpCtr = new Phoenix{};
                enemyCors.push_back(make_pair(j, i));
                theChar = '.';
            } else if (theChar == 'T') {
                tmpCtr = new Troll{};
                enemyCors.push_back(make_pair(j, i));
                theChar = '.';
            } else if (theChar == '@') {
                if (playerChoice == 1) { tmpCtr = new Human(); }
                else if (playerChoice == 2) { tmpCtr = new Dwarf(); }
                else if (playerChoice == 3) { tmpCtr = new Elves(); }
                else if (playerChoice == 4) { tmpCtr = new Orc(); }
                playerCoords = make_pair(j, i);
                theChar = '.';
                playerSpawned = true;
            } else if (theChar == '\\') {
                theLadder = Ladder(make_pair(j, i));
                ladderSpawned = true;
                theChar = '.';
            }
            boardTiles[i].push_back(Cell(theChar, j, i, tmpCtr));
        }
    }
    infile.close();

    // spawns player on a random tile
    // NOTE: to access a tile, do boardTiles[y-coord][x-coord]
    if (playerSpawned == false) {
        playerCoords = generateCoords();
        if (playerChoice == 2) {
            boardTiles[playerCoords.second][playerCoords.first].ctrptr = new Dwarf();
        } else if (playerChoice == 3) {
            boardTiles[playerCoords.second][playerCoords.first].ctrptr = new Elves();
        } else if (playerChoice == 4) {
            boardTiles[playerCoords.second][playerCoords.first].ctrptr = new Orc();
        } else {
            boardTiles[playerCoords.second][playerCoords.first].ctrptr = new Human();
        }
    }
    boardTiles[playerCoords.second][playerCoords.first].ctrptr->increaseAttribute(Attribute::GOLD, playerGoldAmt);    

    // spawns the ladder (hidden by default)
    // spawns the ladder (hidden by default)
    if (ladderSpawned == false) {
        theLadder = Ladder(generateCoords());
    }
    
    // spawn potions --------------------------------
    int potionCounter = 10;
    for (int i = 0; i < potionCounter; i++) {
        pair<int, int> potionCoords = generateCoords();
        int potNum = generateRandomInt(6);
        boardTiles[potionCoords.second][potionCoords.first].ctrptr = new Potion(potNum);
    }

    //the generation of gold ------------------------------
    for (int i = 0; i < 10; ++i) {
        int randInt = generateRandomInt(8);
        if (randInt >= 0 && randInt <= 4) {
            spawnGold(generateCoords(), 1);
        } else if (randInt == 5) {
            spawnDragonItem(generateCoords());
        } else {
            spawnGold(generateCoords(), 2);
        }
        spawnDragonItem(generateCoords());
    }

    // the generation of Barrier Suit ------------------------
    if (!barrierSuitFlag && floorNum < 5) {
        int randInt = generateRandomInt(5);
        if (randInt == 0) {
            spawnDragonItem(generateCoords(), DragonItem::SUIT);
            barrierSuitFlag = true;
        }
    } else if (!barrierSuitFlag && floorNum == 5){
        spawnDragonItem(generateCoords(), DragonItem::SUIT);
        barrierSuitFlag = true;
    }
    // test codes for barrier suit
    // for (int i = 0; i < 20; ++i) {
    
    // spawnDragonItem(generateCoords(), DragonItem::SUIT);
    // }

    // spawn enemies ---------------------------------
    numEnemies = 20;
    enemyAlive = numEnemies;
    for (int i = 0; i < numEnemies; i++) {
        int randInt = generateRandomInt(18);
        if (randInt < 4) {          // Werewolf: 2/9
            spawnEnemy(generateCoords(), 'W');
        } else if (randInt < 7) {   // Vampire: 3/18
            spawnEnemy(generateCoords(), 'V');
        } else if (randInt < 12) {  // Goblin: 5/18
           spawnEnemy(generateCoords(), 'N');
        } else if (randInt < 14) {  // Troll: 1/9
           spawnEnemy(generateCoords(), 'T');
        } else if (randInt < 16 ) { // Phoenix: 1/9
            spawnEnemy(generateCoords(), 'X');
        } else {                    // Merchant: 1/9
            spawnEnemy(generateCoords(), 'M');
        }
    }
}

void Board::checkAndGrabGold(Cell& cell) { //check if its gold and pick gold if so and add to gold record of player
    Creature *creature_ptr = cell.ctrptr;
    int amount;
    if (creature_ptr && creature_ptr->getChar() == 'G' && creature_ptr->available()) {
        amount = creature_ptr->getGold();
        delete creature_ptr;
        creature_ptr = nullptr;
        cell.ctrptr = nullptr;
        boardTiles[playerCoords.second][playerCoords.first].ctrptr->increaseAttribute(Attribute::GOLD, amount);
    }
    if (creature_ptr && creature_ptr->getChar() == 'B' && creature_ptr->available()) {
        cout<< "you picked up the barrier suit! " << endl;
        suitUp = true;
        delete creature_ptr;
        creature_ptr = nullptr;
        cell.ctrptr = nullptr;
    }
}

bool Board::movePlayer(string theDir) {
    if (theDir == "no") {
        return movePlayerN();
    } else if (theDir == "so") {
        return movePlayerS();
    } else if (theDir == "ea") {
        return movePlayerE();
    } else if (theDir == "we") {
        return movePlayerW();
    } else if (theDir == "ne") {
        return movePlayerNE();
    } else if (theDir == "nw") {
        return movePlayerNW();
    } else if (theDir == "se") {
        return movePlayerSE();
    } else if (theDir == "sw") {
        return movePlayerSW();
    }
    return false;
}

int Board::grabPotion(string theDir) {
    if (theDir == "no") {
        return grabPotionN();
    }  else if (theDir == "so") {
        return grabPotionS();
    } else if (theDir == "ea") {
        return grabPotionE();
    } else if (theDir == "we") {
        return grabPotionW();
    } else if (theDir == "ne") {
        return grabPotionNE();
    } else if (theDir == "nw") {
        return grabPotionNW();
    } else if (theDir == "se") {
        return grabPotionSE();
    } else if (theDir == "sw") {
        return grabPotionSW();
    }
    return -1;
}

void Board::moveEnemy() {
    for (auto &i : enemyCors) {
        int direction = generateRandomInt(4); //0 stands for North ... 3 stands for West
        moveSingleEnemy(i, direction);
    }
}

void Board::moveSingleEnemy(pair<int, int> &i, int direction) {
     switch (direction)
        {
        case 0:
            if (!moveCreatureN(i)) moveSingleEnemy(i, generateRandomInt(4));
            break;
        case 1:
            if (!moveCreatureE(i)) moveSingleEnemy(i, generateRandomInt(4));
            break;
        case 2:
            if (!moveCreatureS(i)) moveSingleEnemy(i, generateRandomInt(4));
            break;
        case 3:
            if (!moveCreatureW(i)) moveSingleEnemy(i, generateRandomInt(4));
            break;
        default:
            break;
        }
}


// Generates (x, y) coordinates
// remember - to access a tile, do boardTiles[y-coord][x-coord]
// NOTE: does not spawn within a 1-tile radius of the player
pair<int, int> Board::generateCoords() {
    int counter = 0;
    vector<pair<int, int>> pairVec;
    for (int i = 0; i < boardHeight; i++) {
        for (int j = 0; j < boardWidth; j++) {
            if (boardTiles[i][j].ctrptr == nullptr && boardTiles[i][j].tileVal == '.' 
                && ((j < playerCoords.first - 1 || j > playerCoords.first + 1) 
                || (i < playerCoords.second - 1 || i > playerCoords.second + 1))) {
                    pairVec.push_back(make_pair(j, i));
                    counter++;
            }
        }
    }
    int randPos = generateRandomInt(counter);   
    return pairVec[randPos];
}

int Board::getPlayerHp() {
    return boardTiles[playerCoords.second][playerCoords.first].ctrptr->getHp();
}
int Board::getPlayerAtk() {
    return boardTiles[playerCoords.second][playerCoords.first].ctrptr->getAtk();
}
int Board::getPlayerDef() {
    return boardTiles[playerCoords.second][playerCoords.first].ctrptr->getDef();
}
int Board::getPlayerGold() {
    return boardTiles[playerCoords.second][playerCoords.first].ctrptr->getGold();
}
string Board::getPlayerRace() {
    return boardTiles[playerCoords.second][playerCoords.first].ctrptr->getRace();
}

int Board::applyPotion(int n) {
    Cell &c = boardTiles[playerCoords.second][playerCoords.first];
    if (n < 1) {
        if (c.ctrptr->getHp() == c.ctrptr->getMax_health()) {
            cout << "Max HP reached!" << endl;
        }
        c.ctrptr->increaseAttribute(HP, 10);
    } else if (n < 2) {
        c.ctrptr->increaseAttribute(ATK, 5);
    } else if (n < 3) {
        c.ctrptr->increaseAttribute(DEF, 5);
    } else if (n < 4) {
        c.ctrptr->decreaseAttribute(POISON, 10);
    } else if (n < 5) {
        c.ctrptr->decreaseAttribute(ATK, 5);
    } else {
        c.ctrptr->decreaseAttribute(DEF, 5);
    }
    return n;
}

// for printing the board
ostream &operator<<(ostream &out, Board b) {
    cout << "\033[2J\033[1;1H";
    char ctrC;
    for (int i = 0; i < b.boardHeight; i++) {
        for (int j = 0; j < b.boardWidth; j++) {
            if (b.theLadder.isVisible() && j == b.theLadder.getXCoord() && i == b.theLadder.getYCoord()) {
                out << "\\";
            } else if (b.boardTiles[i][j].ctrptr == nullptr) {
                out << b.boardTiles[i][j].tileVal;
            } else {
                ctrC = b.boardTiles[i][j].ctrptr->getChar();
                if (ctrC == '@') { out << "\033[36m"; }
                else if (ctrC == 'C' || ctrC == '\\') { out << "\033[34m"; }
                else if (ctrC == 'B') { out << "\033[35m"; }
                else if (ctrC == 'G') { out << "\033[33m"; }
                else if (ctrC == 'P') { out << "\033[32m"; }
                else { out << "\033[31m"; }
                out << ctrC << "\033[0m";
            }
            // add else-if statements for player and other creatures
        }
        out << endl;
    }
    return out;
}

// utility commands ----------------------------------------------------------------------------
bool Board::floorCleared() {
    if (theLadder.getXCoord() == playerCoords.first &&
        theLadder.getYCoord() == playerCoords.second) {
        return true;
    }
    return false;
}

void Board::dropCompass(pair<int, int> X) {
    boardTiles[X.second][X.first].ctrptr = new Compass();
    compassSpawned = true;
}

void Board::grabCompass(int X, int Y) {
    // change this to smart pointer later
    Creature *tmpptr = boardTiles[Y][X].ctrptr;
    boardTiles[Y][X].ctrptr = nullptr;
    delete(tmpptr);
    compassSpawned = false;
    theLadder.makeVisible();
}

// movement commands ---------------------------------------------------------------------------
// north
bool Board::movePlayerN() {
    Cell &currTile = boardTiles[playerCoords.second][playerCoords.first];
    Cell &nextTile = boardTiles[playerCoords.second - 1][playerCoords.first];
    Cell &veryNext = boardTiles[playerCoords.second - 2][playerCoords.first];
    checkAndGrabGold(nextTile);
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'C') {
        grabCompass(playerCoords.first, playerCoords.second - 1);
    }

    if (nextTile.ctrptr == nullptr && (nextTile.tileVal == '.' || nextTile.tileVal == '#')) {
        swap(currTile.ctrptr, nextTile.ctrptr);
        playerCoords.second--;
        return true;
    } else if (nextTile.tileVal == '+' && veryNext.ctrptr == nullptr) {
        swap(currTile.ctrptr, veryNext.ctrptr);
        playerCoords.second = playerCoords.second - 2;
        return true;
    }
    return false;
}

bool Board::moveCreatureN(pair<int, int> &Cor) {
    Cell &currTile = boardTiles[Cor.second][Cor.first];
    Cell &nextTile = boardTiles[Cor.second - 1][Cor.first];

    if (nextTile.ctrptr == nullptr && nextTile.tileVal == '.' ) {
        swap(currTile.ctrptr, nextTile.ctrptr);
        Cor.second--;
        return true;
    } 
    return false;
}

// east
bool Board::movePlayerE() {
    Cell &currTile = boardTiles[playerCoords.second][playerCoords.first];
    Cell &nextTile = boardTiles[playerCoords.second][playerCoords.first + 1];
    Cell &veryNext = boardTiles[playerCoords.second][playerCoords.first + 2];
    
    checkAndGrabGold(nextTile);
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'C') {
        grabCompass(playerCoords.first + 1, playerCoords.second);
    }

    if (nextTile.ctrptr == nullptr && (nextTile.tileVal == '.' || nextTile.tileVal == '#')) {
        swap(currTile.ctrptr, nextTile.ctrptr);
        playerCoords.first++;
        return true;
    } else if (nextTile.tileVal == '+' && veryNext.ctrptr == nullptr) {
        swap(currTile.ctrptr, veryNext.ctrptr);
        playerCoords.first = playerCoords.first + 2;
        return true;
    }
    return false;
}

bool Board::moveCreatureE(pair<int, int> &Cor) {
    Cell &currTile = boardTiles[Cor.second][Cor.first];
    Cell &nextTile = boardTiles[Cor.second][Cor.first + 1];

    if (nextTile.ctrptr == nullptr && nextTile.tileVal == '.' ) {
        swap(currTile.ctrptr, nextTile.ctrptr);
        Cor.first++;
        return true;
    } 
    return false;
}


// south
bool Board::movePlayerS() {
    Cell &currTile = boardTiles[playerCoords.second][playerCoords.first];
    Cell &nextTile = boardTiles[playerCoords.second + 1][playerCoords.first];
    Cell &veryNext = boardTiles[playerCoords.second + 2][playerCoords.first];

    checkAndGrabGold(nextTile);
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'C') {
        grabCompass(playerCoords.first, playerCoords.second + 1);
    }

    if (nextTile.ctrptr == nullptr && (nextTile.tileVal == '.' || nextTile.tileVal == '#')) {
        swap(currTile.ctrptr, nextTile.ctrptr);
        playerCoords.second++;
        return true;
    } else if (nextTile.tileVal == '+' && veryNext.ctrptr == nullptr) {
        swap(currTile.ctrptr, veryNext.ctrptr);
        playerCoords.second = playerCoords.second + 2;
        return true;
    }
    return false;
}

bool Board::moveCreatureS(pair<int, int> &Cor) {
    Cell &currTile = boardTiles[Cor.second][Cor.first];
    Cell &nextTile = boardTiles[Cor.second + 1][Cor.first];

    if (nextTile.ctrptr == nullptr && nextTile.tileVal == '.' ) {
        swap(currTile.ctrptr, nextTile.ctrptr);
        Cor.second++;
        return true;
    }
    return false;
}

// west
bool Board::movePlayerW() {
    Cell &currTile = boardTiles[playerCoords.second][playerCoords.first];
    Cell &nextTile = boardTiles[playerCoords.second][playerCoords.first - 1];
    Cell &veryNext = boardTiles[playerCoords.second][playerCoords.first - 2];

    checkAndGrabGold(nextTile);
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'C') {
        grabCompass(playerCoords.first - 1, playerCoords.second);
    }

    if (nextTile.ctrptr == nullptr && (nextTile.tileVal == '.' || nextTile.tileVal == '#')) {
        swap(currTile.ctrptr, nextTile.ctrptr);
        playerCoords.first--;
        return true;
    } else if (nextTile.tileVal == '+' && veryNext.ctrptr == nullptr) {
        swap(currTile.ctrptr, veryNext.ctrptr);
        playerCoords.first = playerCoords.first - 2;
        return true;
    }
    return false;
}

bool Board::moveCreatureW(pair<int, int> &Cor) {
    Cell &currTile = boardTiles[Cor.second][Cor.first];
    Cell &nextTile = boardTiles[Cor.second][Cor.first - 1];

    if (nextTile.ctrptr == nullptr && nextTile.tileVal == '.') {
        swap(currTile.ctrptr, nextTile.ctrptr);
        Cor.first--;
        return true;
    } 
    return false;
}

// north-east
bool Board::movePlayerNE() {
    Cell &currTile = boardTiles[playerCoords.second][playerCoords.first];
    Cell &nextTile = boardTiles[playerCoords.second - 1][playerCoords.first + 1];

    checkAndGrabGold(nextTile);
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'C') {
        grabCompass(playerCoords.first + 1, playerCoords.second - 1);
    }

    if (nextTile.ctrptr == nullptr && nextTile.tileVal == '.') {
        swap(currTile.ctrptr, nextTile.ctrptr);
        playerCoords.first++;
        playerCoords.second--;
        return true;
    }
    return false;
}

// south-east
bool Board::movePlayerSE() {
    Cell &currTile = boardTiles[playerCoords.second][playerCoords.first];
    Cell &nextTile = boardTiles[playerCoords.second + 1][playerCoords.first + 1];

    checkAndGrabGold(nextTile);
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'C') {
        grabCompass(playerCoords.first + 1, playerCoords.second + 1);
    }

    if (nextTile.ctrptr == nullptr && nextTile.tileVal == '.') {
        swap(currTile.ctrptr, nextTile.ctrptr);
        playerCoords.first++;
        playerCoords.second++;
        return true;
    }
    return false;
}

// south-west
bool Board::movePlayerSW() {
    Cell &currTile = boardTiles[playerCoords.second][playerCoords.first];
    Cell &nextTile = boardTiles[playerCoords.second + 1][playerCoords.first - 1];

    checkAndGrabGold(nextTile);
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'C') {
        grabCompass(playerCoords.first - 1, playerCoords.second + 1);
    }

    if (nextTile.ctrptr == nullptr && nextTile.tileVal == '.') {
        swap(currTile.ctrptr, nextTile.ctrptr);
        playerCoords.first--;
        playerCoords.second++;
        return true;
    }
    return false;
}

// north-west
bool Board::movePlayerNW() {
    Cell &currTile = boardTiles[playerCoords.second][playerCoords.first];
    Cell &nextTile = boardTiles[playerCoords.second - 1][playerCoords.first - 1];

    checkAndGrabGold(nextTile);
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'C') {
        grabCompass(playerCoords.first - 1, playerCoords.second - 1);
    }

    if (nextTile.ctrptr == nullptr && nextTile.tileVal == '.') {
        swap(currTile.ctrptr, nextTile.ctrptr);
        playerCoords.first--;
        playerCoords.second--;
        return true;
    }
    return false;
}

// potions -------------------------------------------------------------------
int Board::grabPotionN() {
    Cell &nextTile = boardTiles[playerCoords.second - 1][playerCoords.first];
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'P') {
        int result = nextTile.ctrptr->getPotType();
        deleteEnemy(nextTile);
        return result;
    }
    return -1;
}

int Board::grabPotionS() {
    Cell &nextTile = boardTiles[playerCoords.second + 1][playerCoords.first];
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'P') {
        int result = nextTile.ctrptr->getPotType();
        deleteEnemy(nextTile);
        return result;
    }
    return -1;
}

int Board::grabPotionE() {
    Cell &nextTile = boardTiles[playerCoords.second][playerCoords.first + 1];
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'P') {
        int result = nextTile.ctrptr->getPotType();
        deleteEnemy(nextTile);
        return result;
    }
    return -1;
}

int Board::grabPotionW() {
    Cell &nextTile = boardTiles[playerCoords.second][playerCoords.first - 1];
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'P') {
        int result = nextTile.ctrptr->getPotType();
        deleteEnemy(nextTile);
        return result;
    }
    return -1;
}

int Board::grabPotionNE() {
    Cell &nextTile = boardTiles[playerCoords.second - 1][playerCoords.first + 1];
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'P') {
        int result = nextTile.ctrptr->getPotType();
        deleteEnemy(nextTile);
        return result;
    }
    return -1;
}

int Board::grabPotionNW() {
    Cell &nextTile = boardTiles[playerCoords.second - 1][playerCoords.first - 1];
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'P') {
        int result = nextTile.ctrptr->getPotType();
        deleteEnemy(nextTile);
        return result;
    }
    return -1;
}

int Board::grabPotionSE() {
    Cell &nextTile = boardTiles[playerCoords.second + 1][playerCoords.first + 1];
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'P') {
        int result = nextTile.ctrptr->getPotType();
        deleteEnemy(nextTile);
        return result;
    }
    return -1;
}

int Board::grabPotionSW() {
    Cell &nextTile = boardTiles[playerCoords.second + 1][playerCoords.first - 1];
    if (nextTile.ctrptr != nullptr && nextTile.ctrptr->getChar() == 'P') {
        int result = nextTile.ctrptr->getPotType();
        deleteEnemy(nextTile);
        return result;
    }
    return -1;
}


// ---------------------------------------------------------------------------
void Board::deleteEnemy(Cell &c) {
    vector<pair<int, int>>:: iterator position = find(enemyCors.begin(), enemyCors.end(), c.coords);
    if (position != enemyCors.end()) // == enemyCor.end() means the element was not found
        enemyCors.erase(position);
    Creature *tmpC = c.ctrptr;
    c.ctrptr = nullptr;
    delete tmpC;
}

void Board::spawnEnemy(pair<int, int> cor, char name) {
    Enemy* enemy_new = nullptr;
    switch (name)
    {
    case 'V':
        enemy_new = new Vampire{};
        break;
    case 'T':
        enemy_new = new Troll{};
        break;
    case 'W':
        enemy_new = new Werewolf{};
        break;
    case 'X':
        enemy_new = new Phoenix{};;
        break;
    case 'N':
        enemy_new = new Goblin{};
        break;
    case 'M':
        enemy_new = new Merchant{};
        break;
    default:
        break;
    }
    boardTiles[cor.second][cor.first].ctrptr = enemy_new;
    enemyCors.push_back(cor);
    // cout <<  boardTiles[cor.second][cor.first].ctrptr->getChar() << endl;
} 

void Board::spawnGold(pair<int, int> cor, int amount, Dragon* dragonPtr) {
    Gold* new_gold_ptr = new Gold{amount, dragonPtr};
    boardTiles[cor.second][cor.first].ctrptr = new_gold_ptr;
    goldCors.push_back(cor);
}

Dragon* Board::spawnDragon(pair<int, int> cor) {
    Dragon* new_dragon_ptr = new Dragon{};
    boardTiles[cor.second][cor.first].ctrptr = new_dragon_ptr;
    dragonCors.push_back(cor);
    return new_dragon_ptr;
}

void Board::spawnBarrierSuit(pair<int, int> cor_GS, Dragon* dragonPtr) {
    Barriersuit* new_barriersuit_ptr = new Barriersuit{dragonPtr};
    boardTiles[cor_GS.second][cor_GS.first].ctrptr = new_barriersuit_ptr;
}

void Board::spawnDragonItem(pair<int, int> cor_item, DragonItem item_name) {
    // pair<int, int> cor_gold = generateCoords();
    pair<int, int> cor_dragon;
    int direction = generateRandomInt(4);
    switch (direction)
    {
    case 0:
        cor_dragon.first = cor_item.first;
        cor_dragon.second = cor_item.second - 1;
        break;
    case 1:
        cor_dragon.first = cor_item.first + 1;
        cor_dragon.second = cor_item.second;
        break;
    case 2:
        cor_dragon.first = cor_item.first;
        cor_dragon.second = cor_item.second + 1;
        break;
    case 3:
        cor_dragon.first = cor_item.first - 1;
        cor_dragon.second = cor_item.second;
        break;
    default:
        break;
    }
    Cell tileDragon = boardTiles[cor_dragon.second][cor_dragon.first];
    if (item_name == DragonItem::SUIT) {
        if (tileDragon.ctrptr == nullptr && tileDragon.tileVal == '.') {
            spawnBarrierSuit(cor_item, spawnDragon(cor_dragon));
        } else {
            spawnDragonItem(generateCoords(), DragonItem::SUIT);
        }
    } else {
        if (tileDragon.ctrptr == nullptr && tileDragon.tileVal == '.') {
            spawnGold(cor_item, 6, spawnDragon(cor_dragon));
        } else {
            spawnDragonItem(generateCoords(), DragonItem::GOLDD);
        }
    }
}

int Board::generateRandomInt(int range) {
    std::vector<int> v;
    for (int i = 0; i < range; ++i) {
        v.push_back(i);
    }
    //shuffle vector v randomly
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng{seed};
    std::shuffle( v.begin(), v.end(), rng );
    // cout << v[0] << endl; // to check validity
    return v[0]; //return the first element of the shuffled vector
}
