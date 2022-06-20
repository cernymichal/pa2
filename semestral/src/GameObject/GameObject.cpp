#include "GameObject.h"

GameObject::GameObject() {
}

GameObject::GameObject(const Vector2<uint8_t>& location) : m_location(location) {
}

GameObject::~GameObject() {
}

const Vector2<uint8_t>& GameObject::location() const {
    return m_location;
}

bool GameObject::dead() const {
    return m_dead;
}

void GameObject::kill() {
    m_dead = true;
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
    stream << static_cast<unsigned short>(m_location.x)
           << ' ' << static_cast<unsigned short>(m_location.y)
           << ' ' << static_cast<unsigned short>(m_color)
           << ' ' << m_dead
           << ' ' << m_hitDistance;

    return !stream.fail();
}

bool GameObject::unserialize(std::istream& stream) {
    unsigned short temp;

    stream >> temp;
    m_location.x = temp;

    stream >> temp;
    m_location.y = temp;

    stream >> temp;
    m_color = temp;

    stream >> m_dead;

    stream >> m_hitDistance;

    return !stream.fail();
}

std::ostream& GameObject::log(std::ostream& stream) const {
    return stream << typeid(*this).name()
                  << " x: " << static_cast<unsigned short>(m_location.x)
                  << ", y: " << static_cast<unsigned short>(m_location.y)
                  << ", updatePriority: " << static_cast<unsigned short>(updatePriority());
}
