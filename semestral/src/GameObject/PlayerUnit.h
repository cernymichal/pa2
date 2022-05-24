#pragma once

#include "GameObject.h"
#include "Player.h"

class PlayerUnit : public GameObject {
public:
    explicit PlayerUnit(Player* player = nullptr);

    PlayerUnit(uint8_t x, uint8_t y, Player* player = nullptr);

    Player* player();

    void changePlayer(Player* player);

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;

private:
    Player* _player = nullptr;
    uint8_t _playerId = -1;
};
