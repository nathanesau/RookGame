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

class Cpu
{
    // player variables
    const int &playerNum;
    int &teamNum;
    CardVector &cardArr;
    int &bid;
    bool &passed;

public:
    Cpu(const int &pPlayerNum, int &pTeamNum, CardVector &pCardArr, int &pBid, bool &pPassed);
    void clear();

    int getBid() const;
    CardVector getChosenNest() const;
    int getChosenTrump() const;
    Card getChosenPartner() const;
    Card getCardToPlay() const;

private:
    int getPartnerNum() const;
};

#endif