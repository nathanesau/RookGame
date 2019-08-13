#include <algorithm>
#include <numeric>

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
    
    // DO NOT REMOVE
    for(auto teamNum : vector<int>{TEAM_1, TEAM_2})
    {
        teams[teamNum].clear();
    }

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

    teams[TEAM_1].insert(bidPlayer);
    teams[TEAM_1].insert(partnerPlayerNum);

    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if (teams[TEAM_1].find(playerNum) == teams[TEAM_1].end())
        {
            teams[TEAM_2].insert(playerNum);
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
    for(auto teamNum : vector<int>{TEAM_1, TEAM_2})
    {
        auto &team = teams[teamNum];
        
        if (std::find(team.begin(), team.end(), playerNum) != team.end())
        {
            return teamNum;
        }
    }

    return TEAM_UNDEFINED;
}

void RoundInfo::updatePlayerScores(HandInfo &handInfo)
{
    int winningPlayerNum = handInfo.getWinningPlayerNum(*this);

    playerScores[winningPlayerNum] += handInfo.points;
}

void RoundInfo::updateTeamScores()
{
    if (std::all_of(teams.begin(), teams.end(), [](Team &team) { return team.empty(); })) // unknown teams
    {
        return;
    }

    for(auto teamNum : vector<int>{TEAM_1, TEAM_2})
    {
        auto playerNumArr = vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4};
        teamScores[teamNum] = accumulate(playerNumArr.begin(), playerNumArr.end(), 0, [&](int a, int &b) {
            return (getTeamNumber(b) == teamNum) ? a + playerScores[b] : a;
        });
    }
}