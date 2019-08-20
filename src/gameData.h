#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <array>
#include <map>

#include "card.h"
#include "common.h"
#include "handInfo.h"
#include "overallInfo.h"
#include "player.h"
#include "roundInfo.h"

using namespace std;

struct GameData
{
    map<int, map<int, int>> scoreHistory;
    OverallInfo overallInfo;
    RoundInfo roundInfo;
    DetailedRoundInfo detailRoundInfo;
    HandInfo handInfo;
    CardVector nest;
    array<Player, 4> playerArr;

    GameData();

    void clear();
    void clearRoundSpecificInfo();
    void clearOverallInfo();
};

#endif