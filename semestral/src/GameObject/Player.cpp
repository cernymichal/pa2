#include "Player.h"

#include "../Game.h"

Player::Player() {
}

Player::Player(uint8_t id, uint8_t color, const std::string& name) : id(id), name(name) {
    this->color = color;
}

void Player::onAdd() {
    _game->playerMap[id] = this;
}

bool Player::serialize(std::ostream& stream) const {
    return _serialize(stream << "Player ");
}

bool Player::_serialize(std::ostream& stream) const {
    stream << (unsigned short)id << ' ' << name << ' ';
    
    return GameObject::_serialize(stream);
}

bool Player::unserialize(std::istream& stream) {
    unsigned short temp;

    stream >> temp;
    id = temp;

    stream >> name;

    return GameObject::unserialize(stream);
}
