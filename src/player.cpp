#include "gameData.h"
#include "player.h"
#include "settings.h"

Player::Player(int pPlayerNum) : playerNum (pPlayerNum)
{
    clear();
}

void Player::clear()
{
    teamNum = TEAM_UNDEFINED;
    cardArr.clear();
    bid = 0;
    passed = false;
}

int Player::getNextPlayerNum() const
{
    switch (playerNum)
    {
    case PLAYER_1:
        return PLAYER_2;
    case PLAYER_2:
        return PLAYER_3;
    case PLAYER_3:
        return PLAYER_4;
    case PLAYER_4:
        return PLAYER_1;
    default:
        return PLAYER_UNDEFINED;
    }
}

string Player::getPlayerName() const
{
    return Settings::Appearance::readPlayerNames()[playerNum];
}

int Player::getTeamNum() const
{
    for(auto teamNum : vector<int>{TEAM_1, TEAM_2})
    {
        if(gamedata.roundInfo.teams[teamNum].find(playerNum) != gamedata.roundInfo.teams[teamNum].end())
        {
            return teamNum;
        }
    }

    return TEAM_UNDEFINED;
}

string Team::getTeamName() const
{
    if (empty())
    {
        return "???";
    }

    auto playerNames = Settings::Appearance::readPlayerNames();

    string teamName = "";

    for (auto it = begin(); it != end(); it++)
    {
        teamName += (it == begin()) ? playerNames[*it] : (" + " + playerNames[*it]);
    }

    return teamName;
}