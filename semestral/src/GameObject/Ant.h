#pragma once

#include "PlayerUnit.h"

class Ant : public PlayerUnit {
public:
    int8_t tx = 0;
    int8_t ty = 0;

    float mvx;
    float mvy;

    Ant();

    Ant(uint8_t x, uint8_t y, Player* player, int8_t tx = 0, int8_t ty = 0);

    virtual void draw() const override;

    virtual void update() override;

    virtual void collideWith(GameObject& object) override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;
};
