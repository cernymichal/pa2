#include "Player.h"

#include "../Game.h"

Player::Player() {
}

Player::Player(uint8_t id, uint8_t color, const std::string& name) : m_playerId(id), m_playerName(name) {
    this->m_color = color;
}

void Player::onAdd(Game* game) {
    GameObject::onAdd(game);

    m_game->m_playerMap[m_playerId] = this;
}

bool Player::serialize(std::ostream& stream) const {
    return serializeState(stream << "Player ");
}

bool Player::serializeState(std::ostream& stream) const {
    stream << (unsigned short)m_playerId << ' ' << m_playerName << ' ';

    return GameObject::serializeState(stream);
}

bool Player::unserialize(std::istream& stream) {
    unsigned short temp;

    stream >> temp;
    m_playerId = temp;

    stream >> m_playerName;

    return GameObject::unserialize(stream);
}
