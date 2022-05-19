#pragma once

#include "GameObject.h"

class Player : public GameObject {
public:
    Player();

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;
};
