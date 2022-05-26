#pragma once

#include "GameObject.h"

/**
 * @brief decoration
 */
class Wall : public GameObject {
public:
    Wall();

    Wall(uint8_t x, uint8_t y);

    virtual void draw() const override;

    virtual bool serialize(std::ostream& stream) const override;
};
