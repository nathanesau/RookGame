#include <algorithm>

#include "gameData.h"

using namespace std;

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
    handInfo.clear();
    nest.clear();
    for_each(playerArr.begin(), playerArr.end(), [](Player &player) { player.clear(); });
}

void GameData::clearOverallInfo()
{
    scoreHistory.clear();
    overallInfo.clear();
}
