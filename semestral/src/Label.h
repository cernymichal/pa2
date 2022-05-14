#pragma once

#include "GameObject.h"

class Label : public GameObject {
public:
    std::string text;
    bool centered;

    Label(uint8_t x, uint8_t y, const std::string &text, bool centered = false);

    virtual void draw() const override;
    virtual std::ostream &serialize(std::ostream &stream) const override;
};
