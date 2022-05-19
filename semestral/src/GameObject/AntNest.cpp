#include "AntNest.h"

AntNest::AntNest() : AntNest(0, 0) {
}

AntNest::AntNest(uint8_t x, uint8_t y) : GameObject(x, y) {
    // make sure ants are being spawned after ant movement
    updatePriority = 192;
}

void AntNest::draw() const {
    init_pair(1, COLOR_BLACK, color);
    mvaddch(y, x, '#' | COLOR_PAIR(1));
}

bool AntNest::serialize(std::ostream& stream) const {
    return _serialize(stream << "AntNest ");
}

bool AntNest::_serialize(std::ostream& stream) const {
    return GameObject::_serialize(stream);
}

bool AntNest::unserialize(std::istream& stream) {
    return GameObject::unserialize(stream);
}
