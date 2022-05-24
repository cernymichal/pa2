#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer() {
}

ComputerPlayer::ComputerPlayer(uint8_t id, const std::string& name) : Player(id, name) {
}

bool ComputerPlayer::serialize(std::ostream& stream) const {
    return _serialize(stream << "ComputerPlayer ");
}

bool ComputerPlayer::_serialize(std::ostream& stream) const {
    return Player::_serialize(stream);
}

bool ComputerPlayer::unserialize(std::istream& stream) {
    return Player::unserialize(stream);
}
