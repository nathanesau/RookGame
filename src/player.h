#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <string>
#include <set>

#include "card.h"
#include "cpu.h"
#include "common.h"

using namespace std;

// forward declarations
struct GameData;

// global declarations
extern GameData gamedata;

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
    unique_ptr<Cpu> cpu;

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

// do not delete objects through base class pointer
struct Team : public set<int>
{
    using set<int>::set;

public:
    string getTeamName() const;
};

#endif