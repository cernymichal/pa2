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

    /**
     * @brief activate or disable nest side
     *
     * @param[in] nest side
     * @param[in] value activate / disable
     */
    void switchSide(AntNest* nest, bool value);

    /**
     * @param[in] nest
     * 
     * @returns nest opposite of nest
     */
    AntNest* otherNest(AntNest* nest);

    /**
     * @param[in] nest
     * 
     * @returns is other side sending ants?
     */
    bool otherSideActive(AntNest* nest);

    /**
     * @returns both sides are owned by the same player
     */
    bool friendly();

    virtual void draw() const override;

    /**
     * @brief send ants from nests
     */
    virtual void update() override;

    /**
     * @brief set nest pointers from ids and add this to lineMaps in nests
     */
    virtual void onLoad() override;

    virtual bool serialize(std::ostream& stream) const override;

    virtual bool unserialize(std::istream& stream) override;

protected:
    /**
     * @brief serialize without type header
     *
     * format: "{nestAId} {nestAActive} {nestBId} {nestBActive} " + GameObject serialization
     *
     * @param[in] stream output stream
     */
    virtual bool _serialize(std::ostream& stream) const override;
};
