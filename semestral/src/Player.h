#pragma once

#include "GameObject.h"

class Player : public GameObject {
public:
    Player();

    virtual void serialize(std::ostream &stream) const override;
};
