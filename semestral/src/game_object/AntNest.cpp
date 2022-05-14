#include "AntNest.h"

AntNest::AntNest(uint8_t x, uint8_t y) : GameObject(x, y) {
    // make sure ants are being spawned after ant movement
    updatePriority = 192;
}

void AntNest::draw() const {
    init_pair(1, COLOR_BLACK, color);
    mvaddch(y, x, '#' | COLOR_PAIR(1));
}

std::ostream &AntNest::serialize(std::ostream &stream) const {
    // TODO serialize
    
    return stream;
}
