#include "Ant.h"

#include "../log.h"

Ant::Ant() {
}

Ant::Ant(uint8_t x, uint8_t y) : GameObject(x, y) {
}

void Ant::draw() const {
    init_pair(2, color, COLOR_BLACK);
    mvaddch(y, x, 'm' | COLOR_PAIR(2));
}

void Ant::update() {
    // TODO update

    x += 1;
}

bool Ant::serialize(std::ostream& stream) const {
    return _serialize(stream << "Ant ");
}

bool Ant::_serialize(std::ostream& stream) const {
    return GameObject::_serialize(stream);
}

bool Ant::unserialize(std::istream& stream) {
    return GameObject::unserialize(stream);
}
