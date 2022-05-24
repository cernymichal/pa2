#pragma once

#include "GameObject.h"

class AntNest : public GameObject {
public:
    char id = '?';
    bool starting = false;
    uint8_t ants = 0;

    AntNest();

    AntNest(uint8_t x, uint8_t y, char id, bool stating = false);

    virtual void draw() const override;

    virtual void update() override;

    virtual void collideWith(GameObject& object) override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;
};
