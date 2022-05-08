#pragma once

#include "Player.h"

class ComputerPlayer : public Player {
public:
    virtual void serialize(std::ostream &stream) const override;
};
