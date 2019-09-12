#ifndef ROUNDINFO_H
#define ROUNDINFO_H

#include "card.h"
#include "common.h"
#include "player.h"

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
    std::array<Team, 2> teams;
    std::map<int, int> playerScores;
    std::map<int, int> teamScores;

public:
    RoundInfo();
    void clear();

    void updateTeams();
    void updateScores(HandInfo &handInfo);
    void addPointsMiddleToScores(HandInfo &handInfo);

    std::map<int, int> getRoundScores();
    int getTeamNumber(int playerNum);

private:
    void updatePlayerScores(HandInfo &handInfo);
    void updateTeamScores();
};

// used for determining what cpu card to play
class DetailedRoundInfo
{
public:
    // for db load, can be auto determined using player cards
    std::array<CardVector, 4> suitCardsPlayed;

public:
    DetailedRoundInfo();
    void clear();
};

#endif