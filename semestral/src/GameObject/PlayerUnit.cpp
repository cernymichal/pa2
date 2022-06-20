#include "PlayerUnit.h"

#include "../Game.h"

PlayerUnit::PlayerUnit(Player* player) {
    changeOwningPlayer(player);
}

PlayerUnit::PlayerUnit(const Vector2<uint8_t>& location, Player* player) : GameObject(location) {
    changeOwningPlayer(player);
}

Player* PlayerUnit::player() {
    return m_owningPlayer;
}

void PlayerUnit::changeOwningPlayer(Player* player) {
    if (!player)
        m_owningPlayerId = -1;
    else {
        m_owningPlayerId = player->playerId();
        m_color = player->m_color;
    }

    m_owningPlayer = player;
}

void PlayerUnit::onLoad() {
    if (m_owningPlayerId != -1)
        changeOwningPlayer(m_game->m_playerMap[m_owningPlayerId]);
}

bool PlayerUnit::serializeState(std::ostream& stream) const {
    stream << static_cast<unsigned short>(m_owningPlayerId)
           << ' ';

    return GameObject::serializeState(stream);
}

bool PlayerUnit::unserialize(std::istream& stream) {
    unsigned short temp;

    stream >> temp;
    m_owningPlayerId = temp;

    return GameObject::unserialize(stream);
}
