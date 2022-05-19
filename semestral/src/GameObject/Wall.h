#pragma once

#include "GameObject.h"

class Wall : public GameObject {
public:
    Wall();

    Wall(uint8_t x, uint8_t y);

    virtual void draw() const override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;
};
