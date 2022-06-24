#include "Player.h"

#include "../Game.h"

Player::Player() {
}

Player::Player(uint8_t id, uint8_t color, const std::string& name) : m_playerName(name), m_playerId(id) {
    this->m_color = color;
}

uint8_t Player::playerId() const {
    return m_playerId;
}

void Player::onAdd(Game* game) {
    GameObject::onAdd(game);

    m_game->m_playerMap[m_playerId] = this;
}

bool Player::serializeState(std::ostream& stream) const {
    stream << static_cast<unsigned short>(m_playerId)
           << ' ' << m_playerName
           << ' ';

    return GameObject::serializeState(stream);
}

bool Player::unserialize(std::istream& stream) {
    unsigned short temp;

    stream >> temp;
    m_playerId = temp;

    stream >> m_playerName;

    return GameObject::unserialize(stream);
}
