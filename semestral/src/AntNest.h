#pragma once

#include "GameObject.h"

class AntNest : public GameObject {
public:
    AntNest(uint8_t x, uint8_t y);

    virtual void draw() const override;
    virtual void serialize(std::ostream &stream) const override;
};
