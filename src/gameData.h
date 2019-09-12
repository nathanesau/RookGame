#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "card.h"
#include "common.h"
#include "handInfo.h"
#include "overallInfo.h"
#include "player.h"
#include "roundInfo.h"

struct GameData
{
    std::map<int, std::map<int, int>> scoreHistory;
    OverallInfo overallInfo;
    RoundInfo roundInfo;
    DetailedRoundInfo detailRoundInfo;
    HandInfo handInfo;
    CardVector nest;
    std::array<Player, 4> playerArr;

    GameData();

    void clear();
    void clearRoundSpecificInfo();
    void clearOverallInfo();
};

#endif