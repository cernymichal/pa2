#pragma once

#include <memory>

#include "../GameController.h"
#include "GameObject.h"

class PlayerInput;

/**
 * @brief human player
 *
 * has name and id
 */
class Player : public GameObject {
public:
    std::string m_playerName;

    Player();

    Player(uint8_t id, uint8_t color, const std::string& name = "nobody");

    uint8_t playerId() const;

    /**
     * @brief add this to m_playerMap in m_game
     */
    virtual void onAdd(Game* game) override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    std::unique_ptr<GameController> m_gameController;

    /**
     * @brief serialize for game saving
     *
     * format: "{m_playerId} {m_playerName} " + GameObject serialization
     *
     * @param[in] stream output stream
     */
    virtual bool serializeState(std::ostream& stream) const override;

private:
    uint8_t m_playerId = -1;

    friend class PlayerInput;
};
