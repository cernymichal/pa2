#include "ComputerPlayer.h"

#include "../Game.h"
#include "AntLine.h"

ComputerPlayer::ComputerPlayer() {
}

ComputerPlayer::ComputerPlayer(uint8_t id, uint8_t color, const std::string& name) : Player(id, color, name) {
}

void ComputerPlayer::update() {
    _reactionTimer %= 4;
    if (_reactionTimer++ != 0)
        return;

    auto nests = _game->getNests(id);
    nests.sort([](const AntNest* a, const AntNest* b) {
        return a->ants < b->ants;
    });

    PN_DEBUG_ONLY(bool defendingOld = _defending);

    _checkForAttack(nests);

    PN_DEBUG_ONLY(
        if (defendingOld != _defending) {
            if (_defending)
                PN_LOG(name << " defending");

            else
                PN_LOG(name << " expanding");
        });

    if (_defending)
        _defend(nests);
    else
        _expand(nests);
}

void ComputerPlayer::onLoad() {
    if (_focusedNestId != '?')
        _focusedNest = _game->nestMap[_focusedNestId];
}

bool ComputerPlayer::serialize(std::ostream& stream) const {
    return _serialize(stream << "ComputerPlayer ");
}

bool ComputerPlayer::_serialize(std::ostream& stream) const {
    char focusedNestId = '?';
    if (_focusedNest)
        focusedNestId = _focusedNest->id;

    stream << (unsigned short)_reactionTimer << ' ' << _defending << ' ' << focusedNestId << ' ';
    
    return Player::_serialize(stream);
}

bool ComputerPlayer::unserialize(std::istream& stream) {
    unsigned short temp;

    stream >> temp;
    _reactionTimer = temp;

    stream >> _defending >> _focusedNestId;

    return Player::unserialize(stream);
}

void ComputerPlayer::_checkForAttack(std::list<AntNest*>& nests) {
    _defending = false;
    for (const auto& nest : nests) {
        if (nest->contested()) {
            _defending = true;
            _focusedNest = nest;
            break;
        }
    }
}

void ComputerPlayer::_defend(std::list<AntNest*>& nests) {
    _sendAnts();
}

void ComputerPlayer::_expand(std::list<AntNest*>& nests) {
    _focusedNest = nullptr;
    for (const auto& nest : nests) {
        for (const auto& line : nest->lineMap) {
            if (line.second->friendly())
                continue;

            auto otherNest = line.second->otherNest(nest);
            if (!_focusedNest || _focusedNest->ants > otherNest->ants)
                _focusedNest = otherNest;
        }
    }

    if (_focusedNest)
        _sendAnts();
}

void ComputerPlayer::_sendAnts() {
    std::set<char> visitedNests;
    _sendAnts(_focusedNest, visitedNests);
}

void ComputerPlayer::_sendAnts(AntNest* nest, std::set<char>& visitedNests) {
    visitedNests.insert(nest->id);

    for (const auto& line : nest->lineMap) {
        auto otherNest = line.second->otherNest(nest);
        if ((line.second->friendly() || nest == _focusedNest) && visitedNests.find(otherNest->id) == visitedNests.end()) {
            _game->activateLine(this, otherNest, nest);
            _sendAnts(otherNest, visitedNests);
        }
    }
}
