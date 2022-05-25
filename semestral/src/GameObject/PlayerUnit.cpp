#include "PlayerUnit.h"

#include "../Game.h"

PlayerUnit::PlayerUnit(Player* player) {
    changePlayer(player);
}

PlayerUnit::PlayerUnit(uint8_t x, uint8_t y, Player* player) : GameObject(x, y) {
    changePlayer(player);
}

Player* PlayerUnit::player() {
    return _player;
}

void PlayerUnit::changePlayer(Player* player) {
    if (!player)
        _playerId = -1;
    else {
        _playerId = player->id;
        color = player->color;
    }

    _player = player;
}

void PlayerUnit::onLoad() {
    if (_playerId != -1)
        changePlayer(_game->playerMap[_playerId]);
}

bool PlayerUnit::_serialize(std::ostream& stream) const {
    stream << (unsigned short)_playerId << ' ';
    
    return GameObject::_serialize(stream);
}

bool PlayerUnit::unserialize(std::istream& stream) {
    unsigned short temp;

    stream >> temp;
    _playerId = temp;

    return GameObject::unserialize(stream);
}
