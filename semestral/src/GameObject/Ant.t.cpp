#include "Ant.h"

#include <cassert>

#include "../log.h"

void testAnt() {
    PN_LOGH1("testing ant");

    PN_LOGH2("#1");

    Ant a(10, 23, nullptr);

    assert(a.m_x == 10);
    assert(a.m_y == 23);
}
