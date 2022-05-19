#pragma once

#include "Player.h"

class ComputerPlayer : public Player {
public:
    ComputerPlayer();

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;
};
