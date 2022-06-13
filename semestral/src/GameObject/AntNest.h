#pragma once

#include <map>

#include "Player.h"
#include "PlayerUnit.h"

class AntLine;

class AntNest : public PlayerUnit {
public:
    char id = '?';
    bool starting = false;
    uint8_t ants = 0;

    std::map<char, AntLine*> lineMap;

    AntNest();

    AntNest(uint8_t x, uint8_t y, char id, bool starting = false);

    void disableLines();

    bool contested();

    virtual void draw() const override;

    virtual void update() override;

    virtual void onAdd() override;

    virtual void collideWith(GameObject& object) override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    virtual bool _serialize(std::ostream& stream) const override;

private:
    uint8_t _spawnTimer = 0;
};
