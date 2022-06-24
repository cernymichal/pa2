#pragma once

#include "PlayerUnit.h"

/**
 * @brief Ant moving towards target
 *
 * real m_x and m_y are rounded from m_mvx and m_mvy
 */
class Ant : public PlayerUnit {
public:
    Ant();

    Ant(const Vector2<uint8_t>& location, Player* player, Vector2<uint8_t> targetLocation = {0, 0});

    const Vector2<uint8_t>& target() const;

    virtual void draw() const override;

    /**
     * @brief move towards m_targetLocation
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

    virtual bool unserialize(std::istream& stream) override;

protected:
    /**
     * @brief serialize for game saving
     *
     * format: "{m_targetLocation.x} {m_targetLocation.y} {m_mvLocation.x} {m_mvLocation.y} " + PlayerUnit serialization
     *
     * @param[in] stream output stream
     */
    virtual bool serializeState(std::ostream& stream) const override;

private:
    Vector2<uint8_t> m_targetLocation;
    Vector2<double> m_mvLocation;
};
