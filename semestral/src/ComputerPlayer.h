#pragma once

#include "Player.h"

class ComputerPlayer : public Player {
public:
    virtual std::ostream &serialize(std::ostream &stream) const override;
};
