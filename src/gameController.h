#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <algorithm>
#include <array>
#include <assert.h>
#include <map>
#include <set>
#include <vector>

#include "card.h"
#include "gameData.h"
#include "handInfo.h"
#include "overallInfo.h"
#include "player.h"
#include "roundInfo.h"

using namespace std;

// forward declarations
class CpuPlayer;

// global declarations
extern CpuPlayer cpu;

class GameController
{
public:
    GameData data;

public:
    GameController();

    void startNewRound(); // deal cards

    void getCpuBids(); // get PLAYER_2, PLAYER_3, PLAYER_4 bids

    int getNumPassed(); // get number of playing who passed bid

    void setRookSuitToTrump(); // find rook card and modify suit value

    void playCard(Card cardPlayed, int playerNum); // update hand info

    bool isRoundOver();
};

#endif