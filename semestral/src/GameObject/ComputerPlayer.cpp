#include "ComputerPlayer.h"

#include "../Game.h"
#include "AntLine.h"

ComputerPlayer::ComputerPlayer() {
}

ComputerPlayer::ComputerPlayer(uint8_t id, uint8_t color, const std::string& name) : Player(id, color, name) {
}

void ComputerPlayer::update() {
    m_reactionTimer %= 4;
    if (m_reactionTimer++ != 0)
        return;

    auto nests = m_game->getNests(playerId());
    nests.sort([](const AntNest* a, const AntNest* b) {
        return a->ants() < b->ants();
    });

    PN_DEBUG_ONLY(bool defendingOld = m_defending);

    checkForAttack(nests);

    PN_DEBUG_ONLY(
        if (defendingOld != m_defending) {
            if (m_defending)
                PN_LOG(m_playerName << " defending");

            else
                PN_LOG(m_playerName << " expanding");
        });

    if (m_defending)
        defend(nests);
    else
        expand(nests);
}

void ComputerPlayer::onLoad() {
    if (m_focusedNestId != '?')
        m_focusedNest = m_game->m_nestMap[m_focusedNestId];
}

bool ComputerPlayer::serialize(std::ostream& stream) const {
    return serializeState(stream << "ComputerPlayer ");
}

bool ComputerPlayer::serializeState(std::ostream& stream) const {
    char focusedNestId = '?';
    if (m_focusedNest)
        focusedNestId = m_focusedNest->nestId();

    stream << static_cast<unsigned short>(m_reactionTimer)
           << ' ' << m_defending
           << ' ' << focusedNestId
           << ' ';

    return Player::serializeState(stream);
}

bool ComputerPlayer::unserialize(std::istream& stream) {
    unsigned short temp;

    stream >> temp;
    m_reactionTimer = temp;

    stream >> m_defending >> m_focusedNestId;

    return Player::unserialize(stream);
}

void ComputerPlayer::checkForAttack(std::list<AntNest*>& nests) {
    m_defending = false;
    for (const auto& nest : nests) {
        if (nest->contested()) {
            m_defending = true;
            m_focusedNest = nest;
            break;
        }
    }
}

void ComputerPlayer::defend(std::list<AntNest*>& nests) {
    sendAnts();
}

void ComputerPlayer::expand(std::list<AntNest*>& nests) {
    m_focusedNest = nullptr;
    for (const auto& nest : nests) {
        for (const auto& line : nest->m_lineMap) {
            if (line.second->friendly())
                continue;

            auto otherNest = line.second->otherNest(nest);
            if (!m_focusedNest || m_focusedNest->ants() > otherNest->ants())
                m_focusedNest = otherNest;
        }
    }

    if (m_focusedNest)
        sendAnts();
}

void ComputerPlayer::sendAnts() {
    std::set<char> visitedNests;
    sendAnts(m_focusedNest, visitedNests);
}

void ComputerPlayer::sendAnts(AntNest* nest, std::set<char>& visitedNests) {
    visitedNests.insert(nest->nestId());

    for (const auto& line : nest->m_lineMap) {
        auto otherNest = line.second->otherNest(nest);
        if ((line.second->friendly() || nest == m_focusedNest) && visitedNests.find(otherNest->nestId()) == visitedNests.end()) {
            m_game->activateLine(this, otherNest, nest);
            sendAnts(otherNest, visitedNests);
        }
    }
}
