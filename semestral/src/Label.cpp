#include "Label.h"

Label::Label(uint8_t x, uint8_t y, const std::string &text, bool centered) : GameObject(x, y), text(text), centered(centered) {
}

void Label::draw() const {
    init_pair(2, color, COLOR_BLACK);
    attron(A_BOLD | COLOR_PAIR(2));

    int x = this->x;
    if (centered)
        x -= text.length() / 2;

    mvaddstr(y, x, text.c_str());

    attron(A_NORMAL | COLOR_PAIR(1));
}

void Label::serialize(std::ostream &stream) const {
    // todo
}
