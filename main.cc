#include <unistd.h>
#include <iostream>
#include "board.h"
#include "combat.h"
using namespace std;

int main(int argc, char *argv[]) {
    // declare variables
    Board b;
    string theInput;
    string cmdArg = "board.txt";
    int floorNum = 1;
    int floorsForWin = 5;
    int playerChoice = 1;
    bool barrierSuitFlag = false;

    while(1) {
        cout << "\033[2J\033[1;1H";
        cout << "Welcome to CC3k! To get started, please select a class (1-4)" << endl;
        cout << "Option 1: Human" << endl;
        cout << "Option 2: Dwarf" << endl;
        cout << "Option 3: Elf" << endl;
        cout << "Option 4: Orc" << endl;
        cout << "Your selection: ";
        cin >> theInput;
        if ("1" <= theInput && theInput <= "4") {
            playerChoice = stoi(theInput);
            break;
        } else if (theInput == "q") {
            return 0;
        } else {
            cout << "Invalid selection! Please try again." << endl;
            sleep(1);
        }
    }

    // initialize board
    if (argc > 1){
        cmdArg  = argv[1];
    }
    b.makeBoard(cmdArg, playerChoice, barrierSuitFlag, floorNum);

    // initialize combat
    Combat c = Combat(&b);

    // core game loop
    while(1) {
        // print the board
        cout << b;
        cout << "Race: " << b.getPlayerRace() << "                          ";
        cout << "Gold: " << b.getPlayerGold() << "                          ";
        cout << "Floor: " << floorNum << endl;
        cout << "HP: " << b.getPlayerHp() << endl;
        cout << "Atk: " << b.getPlayerAtk() << endl;
        cout << "Defense: " << b.getPlayerDef() << endl; 
        cout << "Action: ";
        
        // get player input
        cin >> theInput;
        if (theInput == "q") {
            cout << "You quit! Game over!" << endl;
            break;

        // resetting
        } else if (theInput == "r") {
            cout << "Resetting the game!" << endl;
            floorNum = 1;
            sleep(2);
            b.makeBoard(cmdArg, playerChoice, barrierSuitFlag, floorNum);

        // attacking
        } else if (theInput == "a") {
            if(c.attack()) {
                sleep(2);
            } else {
                cout << "You died! Game over!" << endl;
                sleep(2);
                break;
            }

        // for consuming potions
        } else if (theInput == "u") {
            if (!c.attackPlayer() && b.getPlayerHp() <= 0) {
                    cout << "You died! Game over!" << endl;
                    break;
            }
            cin >> theInput;
            int potGrabbed = b.grabPotion(theInput);
            if (potGrabbed == -1) {
                cout << "Error! No potion on this tile - invalid input." << endl;
            } else {
                cout << "Consumed a potion!" << endl;
                int beforeHP = b.getPlayerHp();
                int result = b.applyPotion(potGrabbed);

                if (result == 0) {
                    cout << "It was a healing potion! ";
                    if (beforeHP == b.getPlayerHp()) {
                        cout << "You are already at full hp!" << endl;
                    } else {
                        cout << b.getPlayerHp() - beforeHP << "HP restored!" << endl;
                    }
                } else if (result == 1) {
                    cout << "Recieved an attack boost! +5 ATK!" << endl;
                } else if (result == 2) {
                    cout << "Recieved a defense boost! +5 DEF!" << endl;
                } else if (result == 3) {
                    cout << "It was a poison potion! ";
                    cout <<  beforeHP - b.getPlayerHp() << "HP lost!" << endl;
                    if (b.getPlayerHp() == 1) {
                        cout << "You nearly died from poison, but the power of friendship saved you!" << endl;
                    }
                } else if (result == 4) {
                    cout << "Recieved an attack wound! -5 ATK!" << endl;
                } else {
                    cout << "Recieved a defense wound! -5 DEF!" << endl;
                }
            }
            sleep(1);

        } else {
            if (!b.movePlayer(theInput)) {
                cout << "Error! Invalid input." << endl;
                sleep(1);
            }
            if (!c.attackPlayer() && b.getPlayerHp() <= 0) {
                cout << "You died! Game over!" << endl;
                break;
            }
        }
        b.moveEnemy();
        // check if they cleared a floor
        if (b.floorCleared() == true) {
            cout << "----------------------------------------------------" << endl;
            cout << "You successfully cleared floor " << floorNum << "!"<< endl;
            floorNum++;

            if (floorNum > floorsForWin) {
                cout << "Congratulations! You escaped the dungeon!" << endl;
                cout << "You WIN!" << endl;
                cout << "----------------------------------------------------" << endl;
                break;
            } else {
                cout << "Going to the next floor!" << endl;
            }
            cout << "----------------------------------------------------" << endl;
            sleep(5);
            b.makeBoard(cmdArg, playerChoice, barrierSuitFlag, floorNum);
        }

    }
}
