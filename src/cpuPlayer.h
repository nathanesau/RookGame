#ifndef CPUPLAYER_H
#define CPUPLAYER_H

#include <vector>

#include "card.h"
#include "common.h"

using namespace std;

// forward declarations
struct GameData;

// global declarations
extern GameData gamedata;

class CpuPlayer
{
public:
    CpuPlayer();

    int getBid(int playerNum);
    Card getCardToPlay(int playerNum);
    CardVector getChosenNest(int playerNum);
    int getChosenTrump(int playerNum);
    Card getChosenPartner(int playerNum);

private:
    // helper functions (cpu logic can get quite complicated)
    // consider cpu cache if the code is too time consuming
};

#endif