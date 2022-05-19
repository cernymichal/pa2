#include "Wall.h"

Wall::Wall() {
}

Wall::Wall(uint8_t x, uint8_t y) : GameObject(x, y) {
}

void Wall::draw() const {
    init_pair(2, color, COLOR_BLACK);
    mvaddch(y, x, '#' | COLOR_PAIR(2));
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
