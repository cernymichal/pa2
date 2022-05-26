#pragma once

#include "GameObject.h"
#include "Player.h"

/**
 * @brief GameObject belonging to some Player
 */
class PlayerUnit : public GameObject {
public:
    explicit PlayerUnit(Player* player = nullptr);

    PlayerUnit(uint8_t x, uint8_t y, Player* player = nullptr);

    /**
     * @brief get _player
     */
    Player* player();

    /**
     * @brief change _player pointer and keep _playerId up to date
     */
    void changePlayer(Player* player);

    /**
     * @brief load _player pointer with _playerId
     */
    virtual void onLoad() override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    /**
     * @brief serialize without type header
     *
     * format: "{_playerId} " + GameObject serialization
     *
     * @param[in] stream output stream
     */
    virtual bool _serialize(std::ostream& stream) const override;

private:
    Player* _player = nullptr;
    uint8_t _playerId = -1;
};
