#pragma once

#include "GameObject.h"
#include "Player.h"

class Game {
    friend void GameObject::Register(Game &game) const;
};
