#pragma once

#include <list>
#include <set>

#include "AntNest.h"
#include "Player.h"

class ComputerPlayer : public Player {
public:
    ComputerPlayer();

    ComputerPlayer(uint8_t id, uint8_t color, const std::string& name = "nobodyAI");

    virtual void update() override;

    virtual void onLoad() override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;

private:
    uint8_t _reactionTimer = 0;
    bool _defending = false;
    AntNest* _focusedNest = nullptr;
    char _focusedNestId = '?';

    void _checkForAttack(std::list<AntNest*>& nests);

    void _defend(std::list<AntNest*>& nests);

    void _expand(std::list<AntNest*>& nests);

    void _sendAnts();

    void _sendAnts(AntNest* nest, std::set<char>& visitedNests);
};
