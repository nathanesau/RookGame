#include <algorithm>

#include "handInfo.h"
#include "roundInfo.h"

RoundInfo::RoundInfo()
{
    clear();
}

void RoundInfo::clear()
{
    bidPlayer = PLAYER_UNDEFINED;
    bidAmount = 0;
    trump = SUIT_UNDEFINED;
    pointsMiddle = 0;
    partnerCard = Card(SUIT_UNDEFINED, VALUE_UNDEFINED);
    partnerPlayerNum = PLAYER_UNDEFINED;
    teams.first.clear();
    teams.second.clear();

    // DO NOT REMOVE (iterate through map later)
    for(auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        playerScores[playerNum] = 0;
    }

    // DO NOT REMOVE (iterate through map later)
    for(auto teamNum : vector<int>{TEAM_1, TEAM_2})
    {
        teamScores[teamNum] = 0;
    }
}

void RoundInfo::updateTeams()
{
    if (partnerPlayerNum == PLAYER_UNDEFINED) // cannot set teams (unknown partner)
    {
        return;
    }

    teams.first.insert(bidPlayer);
    teams.first.insert(partnerPlayerNum);

    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if (teams.first.find(playerNum) == teams.first.end())
        {
            teams.second.insert(playerNum);
        }
    }
}

void RoundInfo::updateScores(HandInfo &handInfo)
{
    updatePlayerScores(handInfo);
    updateTeamScores();
}

void RoundInfo::addPointsMiddleToScores(HandInfo &handInfo)
{
    int winningPlayerNum = handInfo.getWinningPlayerNum(*this);

    playerScores[winningPlayerNum] += pointsMiddle;
    teamScores[getTeamNumber(winningPlayerNum)] += pointsMiddle;
}

map<int, int> RoundInfo::getRoundScores()
{
    auto getRoundScore = [this](int playerNum) {
        int team = getTeamNumber(playerNum);

        if (team != TEAM_UNDEFINED)
            return teamScores[team];
        else
            return 0;
    };

    // todo
    // 1. determine which team was bidding team
    // 2. determine if bidding team made their bid (if not, negative score)
    // 3. show negative score in red text

    map<int, int> roundScores;

    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        roundScores[playerNum] = getRoundScore(playerNum);
    }

    return roundScores;
}

int RoundInfo::getTeamNumber(int playerNum)
{
    if (std::find(teams.first.begin(), teams.first.end(), playerNum) != teams.first.end())
    {
        return TEAM_1;
    }
    else if (std::find(teams.second.begin(), teams.second.end(), playerNum) != teams.second.end())
    {
        return TEAM_2;
    }
    else
    {
        return TEAM_UNDEFINED;
    }
}

void RoundInfo::updatePlayerScores(HandInfo &handInfo)
{
    int winningPlayerNum = handInfo.getWinningPlayerNum(*this);

    playerScores[winningPlayerNum] += handInfo.points;
}

void RoundInfo::updateTeamScores()
{
    if (teams.first.empty() || teams.second.empty()) // unknown teams
    {
        return;
    }

    teamScores[TEAM_1] = 0;
    teamScores[TEAM_2] = 0;

    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        teamScores[getTeamNumber(playerNum)] += playerScores[playerNum];
    }
}