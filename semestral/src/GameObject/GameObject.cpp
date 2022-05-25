#include "GameObject.h"

GameObject::GameObject() {
}

GameObject::GameObject(uint8_t x, uint8_t y) : x(x), y(y) {
}

void GameObject::draw() const {
}

void GameObject::update() {
}

void GameObject::onAdd() {
}

void GameObject::onLoad() {
}

void GameObject::collideWith(GameObject& object) {
}

bool GameObject::_serialize(std::ostream& stream) const {
    stream << (unsigned short)x << ' ' << (unsigned short)y << ' ' << (unsigned short)color << ' '
           << dead << ' ' << hitDistance << ' ' << (unsigned short)updatePriority;
    return !stream.fail();
}

bool GameObject::unserialize(std::istream& stream) {
    unsigned short temp;

    stream >> temp;
    x = temp;

    stream >> temp;
    y = temp;

    stream >> temp;
    color = temp;

    stream >> dead;

    stream >> hitDistance;

    stream >> temp;
    updatePriority = temp;

    return !stream.fail();
}

std::ostream& GameObject::log(std::ostream& stream) const {
    return stream << typeid(*this).name() << " x: " << (unsigned short)x << ", y: " << (unsigned short)y
                  << ", updatePriority: " << (unsigned short)updatePriority;
}
