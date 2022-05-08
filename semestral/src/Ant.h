#pragma once

#include "GameObject.h"

class Ant : public GameObject {
public:
    Ant(uint8_t x, uint8_t y);

    virtual void draw() const override;
    virtual void update() override;
    virtual void serialize(std::ostream &stream) const override;
};
