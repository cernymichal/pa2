#include "Wall.h"

Wall::Wall(uint8_t x, uint8_t y) : GameObject(x, y) {
}

void Wall::draw() const {
    init_pair(2, color, COLOR_BLACK);
    mvaddch(y, x, '#' | COLOR_PAIR(2));
}

void Wall::serialize(std::ostream &stream) const {
    // todo
}
