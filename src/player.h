#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "cpu.h"
#include "common.h"

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
    std::unique_ptr<Cpu> cpu;

    const int playerNum;
    int teamNum;
    CardVector cardArr;
    int bid;
    bool passed;

public:
    Player(int pPlayerNum);
    void clear();
 
    int getNextPlayerNum() const;
    std::string getPlayerName() const;
};

// do not delete objects through base class pointer
struct Team : public std::set<int>
{
    using std::set<int>::set;

public:
    std::string getTeamName() const;
};

#endif