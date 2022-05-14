#include "Ant.h"

#include "log.h"

Ant::Ant(uint8_t x, uint8_t y) : GameObject(x, y) {
}

void Ant::draw() const {
    init_pair(2, color, COLOR_BLACK);
    mvaddch(y, x, 'm' | COLOR_PAIR(2));
}

void Ant::update() {
    // todo
}

std::ostream &Ant::serialize(std::ostream &stream) const {
    // todo

    return stream;
}
