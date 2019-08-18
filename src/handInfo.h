#ifndef HANDINFO_H
#define HANDINFO_H

#include <map>

#include "card.h"
#include "common.h"
#include "player.h"

using namespace std;

// forward declarations
class RoundInfo;

struct PlayerCardPair
{
    int playerNum;
    Card card;

    PlayerCardPair(int pPlayerNum, Card pCard) : playerNum(pPlayerNum), card(pCard)
    {
    }
};

class HandInfo
{
public:
    int startingPlayerNum;
    map<int, Card> cardPlayed;
    int suit;
    int points;

public:
    HandInfo();
    void clear();

    PlayerCardPair getWinningPlayerCardPair(const RoundInfo &roundInfo) const;

private:
};

#endif