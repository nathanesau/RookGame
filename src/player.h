#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <set>

#include "card.h"
#include "common.h"

using namespace std;

// typedef declarations
typedef set<int> Team;

// DO NOT MODIFY
const int PLAYER_UNDEFINED = -1;
const int PLAYER_1 = 0;
const int PLAYER_2 = 1;
const int PLAYER_3 = 2;
const int PLAYER_4 = 3;

// DO NOT MODIFY
const int TEAM_UNDEFINED = -1;
const int TEAM_1 = 0;
const int TEAM_2 = 1;

struct Player
{
    const int playerNum;
    int teamNum;
    CardVector cardArr;
    int bid;
    bool passed;

public:
    Player(int pPlayerNum);
    void clear();
 
    int getNextPlayerNum() const;
    string getPlayerName() const;
};

#endif