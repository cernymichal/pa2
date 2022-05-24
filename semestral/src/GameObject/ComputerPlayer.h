#pragma once

#include "Player.h"

class ComputerPlayer : public Player {
public:
    ComputerPlayer();

    ComputerPlayer(uint8_t id, uint8_t color, const std::string& name = "nobodyAI");

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;
};
