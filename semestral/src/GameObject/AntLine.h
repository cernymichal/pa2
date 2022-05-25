#pragma once

#include "AntNest.h"
#include "GameObject.h"

class AntLine : public GameObject {
public:
    AntNest* nestB = nullptr;
    char nestAId;
    bool nestAActive = false;

    AntNest* nestA = nullptr;
    char nestBId;
    bool nestBActive = false;

    AntLine();

    AntLine(char nestA, char nestB);

    void switchSide(AntNest* nest, bool value);

    AntNest* otherNest(AntNest* nest);

    bool otherSideActive(AntNest* nest);

    bool friendly();

    virtual void draw() const override;

    virtual void update() override;

    virtual void onLoad() override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;
};
