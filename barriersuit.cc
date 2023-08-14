#include "barriersuit.h"

bool Barriersuit::available() {
    return (enemyPtr == nullptr);
}

void Barriersuit::makeAvailable() {
    enemyPtr = nullptr;
}
