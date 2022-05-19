#include "Player.h"

Player::Player() {
}

bool Player::serialize(std::ostream& stream) const {
    return _serialize(stream << "Player ");
}

bool Player::_serialize(std::ostream& stream) const {
    return GameObject::_serialize(stream);
}

bool Player::unserialize(std::istream& stream) {
    return GameObject::unserialize(stream);
}
