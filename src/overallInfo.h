#ifndef OVERALLINFO_H
#define OVERALLINFO_H

#include "common.h"

// forward declarations
class RoundInfo;

class OverallInfo
{
public:
    std::map<int, int> playerScores;
    int roundNum;

public:
    OverallInfo();
    void clear();

    void updatePlayerScores(RoundInfo &roundInfo);
};

#endif