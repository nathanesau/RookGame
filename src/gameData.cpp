#include "common.h"
#include "gameData.h"

GameData::GameData() : playerArr{Player(PLAYER_1), Player(PLAYER_2), Player(PLAYER_3), Player(PLAYER_4)}
{
}

void GameData::clear()
{
    clearRoundSpecificInfo();
    clearOverallInfo();
}

void GameData::clearRoundSpecificInfo()
{
    roundInfo.clear();
    detailRoundInfo.clear();
    handInfo.clear();
    nest.clear();
    std::for_each(playerArr.begin(), playerArr.end(), [](Player &player) { player.clear(); });
}

void GameData::clearOverallInfo()
{
    scoreHistory.clear();
    overallInfo.clear();
}
