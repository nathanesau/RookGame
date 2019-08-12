#include "overallInfo.h"
#include "roundInfo.h"

OverallInfo::OverallInfo()
{
    roundNum = 0;
}

void OverallInfo::clear()
{
    playerScores.clear();
    roundNum = 0;
}

void OverallInfo::updatePlayerScores(RoundInfo &roundInfo)
{
    map<int, int> roundScores = roundInfo.getRoundScores();

    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        playerScores[playerNum] += roundScores[playerNum];
    }
}
