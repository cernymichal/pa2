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
     * @brief set m_focusedNest pointer from m_focusedNestId
     */
    virtual void onLoad() override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    /**
     * @brief serialize for game saving
     *
     *  format: "{m_reactionTimer} {m_defending} {focusedNestId} " + Player serialization
     *
     * @param[in] stream output stream
     */
    virtual bool serializeState(std::ostream& stream) const override;

private:
    uint8_t m_reactionTimer = 0;
    bool m_defending = false;
    AntNest* m_focusedNest = nullptr;
    char m_focusedNestId = '?';

    /**
     * @brief check captured nests for incoming attacks
     *
     * focuses attacked nest and sets m_defending = true
     */
    void checkForAttack(std::list<AntNest*>& nests);

    /**
     * @brief send ants towards defended nest
     *
     * @param[in] nests captured nests
     */
    void defend(std::list<AntNest*>& nests);

    /**
     * @brief find weakest available nest and attack
     *
     * @param[in] nests captured nests
     */
    void expand(std::list<AntNest*>& nests);

    /**
     * @brief send ants towards m_focusedNest
     */
    void sendAnts();

    /**
     * @brief recursively activate lines towards nest
     *
     * @param[in] nest current target
     * @param[in] visitedNests nests with already activated lines
     */
    void sendAnts(AntNest* nest, std::set<char>& visitedNests);
};
