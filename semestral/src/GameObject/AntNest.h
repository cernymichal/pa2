#pragma once

#include <map>

#include "Player.h"
#include "PlayerUnit.h"

class AntLine;

class AntNest : public PlayerUnit {
public:
    char id = '?';
    bool starting = false;
    uint8_t ants = 0;

    std::map<char, AntLine*> lineMap;

    AntNest();

    AntNest(uint8_t x, uint8_t y, char id, bool starting = false);

    /**
     * @brief disables all AntLines originating here
     */
    void disableLines();

    /**
     * @returns is fought over?
     */
    bool contested();

    virtual void draw() const override;

    /**
     * @brief periodically spawn ants
     */
    virtual void update() override;

    /**
     * @brief add this to nestMap in _game
     */
    virtual void onAdd() override;

    /**
     * @brief collide with ant
     *
     * kill defending ant or recieve a friendly one
     *
     * @param[in] object collided with
     */
    virtual void collideWith(GameObject& object) override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    /**
     * @brief serialize without type header
     *
     * format: "{id} {starting} {ants} {_spawnTimer} " + PlayerUnit serialization
     *
     * @param[in] stream output stream
     */
    virtual bool _serialize(std::ostream& stream) const override;

private:
    uint8_t _spawnTimer = 0;
};
