#pragma once

#include "PlayerUnit.h"

/**
 * @brief Ant moving towards target
 * 
 * real x and y are rounded from mvx and mvy
 */
class Ant : public PlayerUnit {
public:
    int8_t tx = 0;
    int8_t ty = 0;

    float mvx;
    float mvy;

    Ant();

    Ant(uint8_t x, uint8_t y, Player* player, int8_t tx = 0, int8_t ty = 0);

    virtual void draw() const override;

    /**
     * @brief move towards (tx, ty)
     */
    virtual void update() override;

    /**
     * @brief collide with other ant or nest
     *
     * - either way, die
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
     * format: "{tx} {ty} {mvx} {mvy} " + PlayerUnit serialization
     *
     * @param[in] stream output stream
     */
    virtual bool _serialize(std::ostream& stream) const override;
};
