#pragma once

#include <map>

#include "Player.h"
#include "PlayerUnit.h"

class AntLine;

/**
 * @brief capturable point with Ants inside
 *
 * has id, starting (players should start here) and stationed ant count
 */
class AntNest : public PlayerUnit {
public:
    std::map<char, AntLine*> m_lineMap;

    AntNest();

    AntNest(const Vector2<uint8_t>& location, char id, bool starting = false);

    char nestId() const;

    bool starting() const;

    uint8_t ants() const;

    /**
     * @brief disables all AntLines originating here
     */
    void disableLines();

    /**
     * @returns is fought over?
     */
    bool contested();

    void spawnAnt(const AntNest* targetNest);

    virtual void draw() const override;

    /**
     * @brief periodically spawn ants
     */
    virtual void update() override;

    /**
     * @brief add this to m_nestMap in m_game
     */
    virtual void onAdd(Game* game) override;

    /**
     * @brief collide with ant
     *
     * kill defending ant or recieve a friendly one
     *
     * @param[in] object collided with
     */
    virtual void collideWith(GameObject& object) override;

    /**
     * make sure ants are being spawned after ant movement
     *
     * @returns update priority of object
     */
    virtual uint8_t updatePriority() const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    /**
     * @brief serialize for game saving
     *
     * format: "{m_nestId} {m_starting} {m_ants} {m_spawnTimer} " + PlayerUnit serialization
     *
     * @param[in] stream output stream
     */
    virtual bool serializeState(std::ostream& stream) const override;

private:
    char m_nestId = '?';
    bool m_starting = false;
    uint8_t m_ants = 0;
    uint8_t m_spawnTimer = 0;
};
