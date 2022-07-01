#include "GameObject.h"

#include <cxxabi.h>

GameObject::GameObject() {
}

GameObject::GameObject(const Vector2<uint8_t>& location) : m_location(location) {
}

GameObject::~GameObject() {
}

const Vector2<uint8_t>& GameObject::location() const {
    return m_location;
}

bool GameObject::destroyed() const {
    return m_destroyed;
}

void GameObject::destroy() {
    m_destroyed = true;
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

void GameObject::onErase() {
}

bool GameObject::collisionEnabled() const {
    return m_collision;
}

void GameObject::collideWith(GameObject& object) {
}

uint8_t GameObject::updatePriority() const {
    return 128;
}

std::string GameObject::name() const {
    // https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
    int status;
    auto realName = abi::__cxa_demangle(typeid(*this).name(), 0, 0, &status);
    std::string str(realName);
    free(realName);
    return str;
}

Game* GameObject::game() const {
    return m_game;
}

bool GameObject::serialize(std::ostream& stream) const {
    return serializeState(stream << name() << " ");
}

bool GameObject::serializeState(std::ostream& stream) const {
    stream << static_cast<unsigned short>(m_location.x)
           << ' ' << static_cast<unsigned short>(m_location.y)
           << ' ' << static_cast<unsigned short>(m_color)
           << ' ' << m_destroyed
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

    stream >> m_destroyed;

    stream >> m_hitDistance;

    return !stream.fail();
}

std::ostream& GameObject::log(std::ostream& stream) const {
    return stream << name()
                  << " x: " << static_cast<unsigned short>(m_location.x)
                  << ", y: " << static_cast<unsigned short>(m_location.y)
                  << ", updatePriority: " << static_cast<unsigned short>(updatePriority());
}
