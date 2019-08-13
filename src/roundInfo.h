#ifndef ROUNDINFO_H
#define ROUNDINFO_H

#include <array>
#include <map>
#include <set>

#include "card.h"
#include "common.h"
#include "player.h"

using namespace std;

// forward declarations
struct GameData;
class HandInfo;

// global declarations
extern GameData gamedata;

class RoundInfo
{
public:
    int bidPlayer;
    int bidAmount;
    int trump;
    int pointsMiddle;
    Card partnerCard;
    int partnerPlayerNum;
    array<Team, 2> teams;
    map<int, int> playerScores;
    map<int, int> teamScores;

public:
    RoundInfo();
    void clear();

    void updateTeams();
    void updateScores(HandInfo &handInfo);
    void addPointsMiddleToScores(HandInfo &handInfo);

    map<int, int> getRoundScores();
    int getTeamNumber(int playerNum);

private:
    void updatePlayerScores(HandInfo &handInfo);
    void updateTeamScores();
};

#endif