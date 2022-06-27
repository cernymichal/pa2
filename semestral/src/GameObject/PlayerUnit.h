#pragma once

#include "GameObject.h"
#include "Player.h"

/**
 * @brief GameObject belonging to some Player
 */
class PlayerUnit : public GameObject {
public:
    explicit PlayerUnit(Player* player = nullptr);

    PlayerUnit(const Vector2<uint8_t>& location, Player* player = nullptr);

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
     * @brief serialize for game saving
     *
     * format: "{m_owningPlayerId} " + GameObject serialization
     *
     * @param[in] stream output stream
     */
    virtual bool serializeState(std::ostream& stream) const override;

private:
    Player* m_owningPlayer = nullptr;
    // TODO remove -1
    uint8_t m_owningPlayerId = -1;
};
