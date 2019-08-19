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

const int CPU_BID_AGGRESSION_LEVEL_LOW = 0;
const int CPU_BID_AGGRESSION_LEVEL_MID = 1;
const int CPU_BID_AGRESSION_LEVEL_HIGH = 2;

class CpuDecisionMaker
{
public:
    CpuDecisionMaker();

    int getBid(int playerNum) const;
    CardVector getChosenNest(int playerNum) const;
    int getChosenTrump(int playerNum) const;
    Card getChosenPartner(int playerNum) const;
    Card getCardToPlay(int playerNum) const;

private:
    int getPartnerNum(int playerNum) const;
};

#endif