#pragma once

#include <list>
#include <set>

#include "AntNest.h"
#include "Player.h"

/**
 * @brief AI Player periodically making moves
 *
 * focuses on one AntNest only
 */
class ComputerPlayer : public Player {
public:
    ComputerPlayer();

    ComputerPlayer(uint8_t id, uint8_t color, const std::string& name = "nobodyAI");

    /**
     * @brief make next decision
     */
    virtual void update() override;

    /**
     * @brief set _focusedNest pointer from _focusedNestId
     */
    virtual void onLoad() override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    /**
     * @brief serialize without type header
     *
     * format: "{_reactionTimer} {_defending} {focusedNestId} " + Player serialization
     *
     * @param[in] stream output stream
     */
    virtual bool _serialize(std::ostream& stream) const override;

private:
    uint8_t _reactionTimer = 0;
    bool _defending = false;
    AntNest* _focusedNest = nullptr;
    char _focusedNestId = '?';

    /**
     * @brief check captured nests for incoming attacks
     *
     * focuses attacked nest and sets _defending = true
     */
    void _checkForAttack(std::list<AntNest*>& nests);

    /**
     * @brief send ants towards defended nest
     *
     * @param[in] nests captured nests
     */
    void _defend(std::list<AntNest*>& nests);

    /**
     * @brief find weakest available nest and attack
     *
     * @param[in] nests captured nests
     */
    void _expand(std::list<AntNest*>& nests);

    /**
     * @brief send ants towards _focusedNest
     */
    void _sendAnts();

    /**
     * @brief recursively activate lines towards nest
     *
     * @param[in] nest current target
     * @param[in] visitedNests nests with already activated lines
     */
    void _sendAnts(AntNest* nest, std::set<char>& visitedNests);
};
