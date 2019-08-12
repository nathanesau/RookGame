#ifndef HANDINFO_H
#define HANDINFO_H

#include <map>

#include "card.h"
#include "common.h"
#include "player.h"

using namespace std;

// forward declarations
class RoundInfo;

class HandInfo
{
public:
    Card winningCard;
    int winningPlayerNum;
    int startingPlayerNum;
    map<int, Card> cardPlayed;
    int suit;
    int points;

public:
    HandInfo();
    void clear();

    int getWinningPlayerNum(const RoundInfo &roundInfo);
    Card getWinningCard(const RoundInfo &roundInfo);

private:
    void updateWinningCombination(const RoundInfo &roundInfo);
};

#endif