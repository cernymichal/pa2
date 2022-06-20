#pragma once

#include "GameObject.h"

/**
 * @brief decoration
 */
class Wall : public GameObject {
public:
    Wall();

    Wall(const Vector2<uint8_t>& location);

    virtual void draw() const override;

    virtual bool serialize(std::ostream& stream) const override;
};
