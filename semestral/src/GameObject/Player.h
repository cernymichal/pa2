#pragma once

#include "GameObject.h"

class Player : public GameObject {
public:
    uint8_t id = -1;
    std::string name;

    Player();

    Player(uint8_t id, uint8_t color, const std::string& name = "nobody");

    virtual void onAdd() override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;
};
