#pragma once

#include <iostream>

#include "Game.h"

class GameObject {
public:
    virtual void Draw() const;
    virtual void Update();
    virtual void Serialize(std::ostream &stream) const = 0;
    virtual void Register(Game &game) const;
};
