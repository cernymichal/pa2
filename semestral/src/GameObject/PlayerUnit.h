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
     * @brief get m_owningPlayer
     */
    Player* player();

    /**
     * @brief change m_owningPlayer pointer and keep m_owningPlayerId up to date
     */
    void changeOwningPlayer(Player* player);

    /**
     * @brief load m_owningPlayer pointer with m_owningPlayerId
     */
    virtual void onLoad() override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    /**
     * @brief serialize without type header
     *
     * format: "{m_owningPlayerId} " + GameObject serialization
     *
     * @param[in] stream output stream
     */
    virtual bool serializeState(std::ostream& stream) const override;

private:
    Player* m_owningPlayer = nullptr;
    uint8_t m_owningPlayerId = -1;
};
