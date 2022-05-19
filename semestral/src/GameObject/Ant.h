#pragma once

#include "GameObject.h"

class Ant : public GameObject {
public:
    Ant();

    Ant(uint8_t x, uint8_t y);

    virtual void draw() const override;

    virtual void update() override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;
};
