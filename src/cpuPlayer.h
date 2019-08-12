#ifndef CPUPLAYER_H
#define CPUPLAYER_H

#include <vector>

#include "card.h"
#include "common.h"

using namespace std;

// forward declarations
class GameController;

// global declarations
extern GameController gc;

class CpuPlayer
{
public:
    CpuPlayer();

    int getBid(int playerNum);
    Card getCardToPlay(int playerNum);
    CardVector getChosenNest(int playerNum);
    int getChosenTrump(int playerNum);
    Card getChosenPartner(int playerNum);
};

#endif