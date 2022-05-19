#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer() {
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
