#include "Wall.h"

#include <ncurses.h>

Wall::Wall() {
    hitDistance = 0;
}

Wall::Wall(uint8_t x, uint8_t y) : GameObject(x, y) {
    hitDistance = 0;
}

void Wall::draw() const {
    mvaddch(y, x, '#' | COLOR_PAIR(color));
}

bool Wall::serialize(std::ostream& stream) const {
    return _serialize(stream << "Wall ");
}

bool Wall::_serialize(std::ostream& stream) const {
    return GameObject::_serialize(stream);
}

bool Wall::unserialize(std::istream& stream) {
    return GameObject::unserialize(stream);
}
