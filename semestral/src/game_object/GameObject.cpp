#include "GameObject.h"

GameObject::GameObject(uint8_t x, uint8_t y) : x(x), y(y) {
}

void GameObject::draw() const {
}

void GameObject::update() {
}

std::ostream &GameObject::log(std::ostream &stream) const {
    return stream << typeid(*this).name() << " x: " << (unsigned)x << ", y: " << (unsigned)y << ", updatePriority: " << (unsigned)updatePriority;
}
