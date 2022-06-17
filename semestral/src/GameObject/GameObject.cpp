#include "GameObject.h"

GameObject::GameObject() {
}

GameObject::GameObject(uint8_t x, uint8_t y) : m_x(x), m_y(y) {
}

void GameObject::draw() const {
}

void GameObject::update() {
}

void GameObject::onAdd(Game* game) {
    m_game = game;
}

void GameObject::onLoad() {
}

void GameObject::collideWith(GameObject& object) {
}

uint8_t GameObject::updatePriority() const {
    return 128;
}

bool GameObject::serializeState(std::ostream& stream) const {
    stream << static_cast<unsigned short>(m_x)
           << ' ' << static_cast<unsigned short>(m_y)
           << ' ' << static_cast<unsigned short>(m_color)
           << ' ' << m_dead
           << ' ' << m_hitDistance;

    return !stream.fail();
}

bool GameObject::unserialize(std::istream& stream) {
    unsigned short temp;

    stream >> temp;
    m_x = temp;

    stream >> temp;
    m_y = temp;

    stream >> temp;
    m_color = temp;

    stream >> m_dead;

    stream >> m_hitDistance;

    return !stream.fail();
}

std::ostream& GameObject::log(std::ostream& stream) const {
    return stream << typeid(*this).name()
                  << " x: " << static_cast<unsigned short>(m_x)
                  << ", y: " << static_cast<unsigned short>(m_y)
                  << ", updatePriority: " << static_cast<unsigned short>(updatePriority());
}
