#ifndef DECK_H
#define DECK_H

#include <array>
#include <vector>

#include "card.h"
#include "common.h"
#include "player.h"

using namespace std;

class Deck
{
    CardVector cardArr;
    int seed;

public:
    Deck();

#ifdef QT_DEBUG
    void setSeed(int pSeed);
#endif

    void initialize();
    void deal(array<Player, 4> &playerArr, CardVector &nest);
};

#endif