#pragma once

#include "PlayerUnit.h"

/**
 * @brief Ant moving towards target
 * 
 * real m_x and m_y are rounded from m_mvx and m_mvy
 */
class Ant : public PlayerUnit {
public:
    int8_t m_tx = 0;
    int8_t m_ty = 0;

    float m_mvx;
    float m_mvy;

    Ant();

    Ant(uint8_t x, uint8_t y, Player* player, int8_t tx = 0, int8_t ty = 0);

    virtual void draw() const override;

    /**
     * @brief move towards (m_tx, m_ty)
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
     * format: "{m_tx} {m_ty} {m_mvx} {m_mvy} " + PlayerUnit serialization
     *
     * @param[in] stream output stream
     */
    virtual bool serializeState(std::ostream& stream) const override;
};
