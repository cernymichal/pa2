#pragma once

#include "GameObject.h"

class Player : public GameObject {
public:
    Player();

    virtual std::ostream &serialize(std::ostream &stream) const override;
};
