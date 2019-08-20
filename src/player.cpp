#include <numeric>

#include "cpu.h"
#include "gameData.h"
#include "player.h"
#include "settings.h"

Player::Player(int pPlayerNum) : playerNum(pPlayerNum),
                                 teamNum(TEAM_UNDEFINED),
                                 cardArr(),
                                 bid(0),
                                 passed(false)
{
    // for PLAYER_1, cpu is used for "AUTO" decisions
    cpu = make_unique<Cpu>(playerNum, teamNum, cardArr, bid, passed);
}

void Player::clear()
{
    cpu->clear();
    // playerNum is constant, not cleared
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

string Team::getTeamName() const
{
    if (empty())
        return "???";

    auto names = Settings::Appearance::readPlayerNames();
    string teamName = names[*begin()];
    std::for_each(++begin(), end(), [&](int playerNum) { teamName += " + " + names[playerNum]; });
    return teamName;
}