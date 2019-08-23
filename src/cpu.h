#ifndef CPUPLAYER_H
#define CPUPLAYER_H

#include <set>
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

const int CPU_RANDOMNESS_LEVEL_NONE = 0;
const int CPU_RANDOMNESS_LEVEL_LOW = 1; // low (10%)
const int CPU_RANDOMNESS_LEVEL_MID = 2; // mid (20%)
const int CPU_RANDOMNESS_LEVEL_HIGH = 3; // high (30%)

enum PlayCardStrategy
{
    STRATEGY_UNDEFINED = -1,
    STRATEGY_RANDOM = 0,
    STRATEGY_FEED_POINTS = 1,
    STRATEGY_PLAY_HIGHEST_CARD = 2,
    STRATEGY_THROW_AWAY_CARD = 3
};

// inputs used to determine which strategy to use
struct CpuPlayCardStrategyInputs
{
    const int cpuPlayerNum;

    CardVector playableCards;
    vector<int> partnerNum;

    Card winningCard;
    int winningPlayerNum;

    CardVector winningSuitCardsLeft;
    Card maxWinningSuitCardLeft;

    CardVector trumpCardsLeft;
    Card maxTrumpCardLeft;

    CardVector currentHandCards;

    double probWinningCardTrumpable;

public:
    CpuPlayCardStrategyInputs(const int pCpuPlayerNum);

    bool isWinningPlayerNumLikelyToWinHand();
    bool isPartner(int playerNum);

    bool isHandWorthWinning();
    bool isCpuAbleToWinHand();

private:
    void initializePartnerNum();
    void initializeProbWinningCardTrumpable();
    void initializeCurrentHandCards();
};

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

    void selectNest();    // output: gamedata.nest, this->cardArr
    void selectTrump();   // output: gamedata.roundInfo.trump
    void selectPartner(); // output: gamedata.roundInfo.partnerCard

    Card getCardToPlay() const;

    Card getRandomCardToPlay(CpuPlayCardStrategyInputs &inputs) const;    // STRATEGY_RANDOM
    Card getFeedCardToPlay(CpuPlayCardStrategyInputs &inputs) const;      // STRATEGY_FEED_POINTS
    Card getHighestCardToPlay(CpuPlayCardStrategyInputs &inputs) const;   // STRATEGY_PLAY_HIGHEST_CARD
    Card getThrowAwayCardToPlay(CpuPlayCardStrategyInputs &inputs) const; // STRATEGY_THROW_AWAY_CARD

private:
    bool useRandomStrategy() const;
    PlayCardStrategy getPlayCardStrategy(CpuPlayCardStrategyInputs &inputs) const;
};

#endif