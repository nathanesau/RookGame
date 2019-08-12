#ifndef OVERALLINFO_H
#define OVERALLINFO_H

#include <map>

using namespace std;

// forward declarations
class RoundInfo;

class OverallInfo
{
public:
    map<int, int> playerScores;
    int roundNum;

public:
    OverallInfo();
    void clear();

    void updatePlayerScores(RoundInfo &roundInfo);
};

#endif