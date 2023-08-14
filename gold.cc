#include "gold.h"

bool Gold::available() {
    return (enemyPtr == nullptr);
}

void Gold::makeAvailable() {
    enemyPtr = nullptr;
}
