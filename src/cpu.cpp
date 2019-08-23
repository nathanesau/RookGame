#include "cpu.h"
#include "gameData.h"
#include "settings.h"
#include "utils.h"

#include <map>
#include <set>

using namespace std;

void CpuPlayCardStrategyInputs::initializePartnerNum()
{
    CardVector &cardArr = gamedata.playerArr[cpuPlayerNum].cardArr;
    bool cpuHasPartnerCard = cardArr.hasCard(gamedata.roundInfo.partnerCard);

    if (cpuHasPartnerCard) // partnership is known
    {
        partnerNum = {gamedata.roundInfo.bidPlayer};
        return;
    }
    else // partnership is known only if partner has revealed themself
    {
        for (auto &team : gamedata.roundInfo.teams)
        {
            if (std::find(team.begin(), team.end(), cpuPlayerNum) != team.end())
            {
                std::copy_if(team.begin(), team.end(), std::back_inserter(partnerNum), [this](auto p) { return p != cpuPlayerNum; });
                return;
            }
        }
    }
}

void CpuPlayCardStrategyInputs::initializeProbWinningCardTrumpable()
{
    if (winningCard.suit == gamedata.roundInfo.trump)
    {
        probWinningCardTrumpable = 0;
        return;
    }

    // probability distribution that hand can be trumped
    if (winningSuitCardsLeft.size() >= 10)
    {
        probWinningCardTrumpable = 0;
    }
    else if (winningSuitCardsLeft.size() >= 8)
    {
        probWinningCardTrumpable = 0.25;
    }
    else if (winningSuitCardsLeft.size() >= 7)
    {
        probWinningCardTrumpable = 0.4;
    }
    else if (winningSuitCardsLeft.size() >= 6)
    {
        probWinningCardTrumpable = 0.6;
    }
    else if (winningSuitCardsLeft.size() >= 4)
    {
        probWinningCardTrumpable = 0.8;
    }
    else
    {
        probWinningCardTrumpable = 1.0;
    }
}

void CpuPlayCardStrategyInputs::initializeCurrentHandCards()
{
    for (auto mapIt : gamedata.handInfo.cardPlayed)
    {
        auto &card = mapIt.second;

        if (!card.isUndefined())
        {
            currentHandCards.push_back(card);
        }
    }
}

CpuPlayCardStrategyInputs::CpuPlayCardStrategyInputs(const int pCpuPlayerNum) : cpuPlayerNum(pCpuPlayerNum)
{
    CardVector &cardArr = gamedata.playerArr[cpuPlayerNum].cardArr;
    playableCards = cardArr.getPlayableCards(gamedata.handInfo);

    initializePartnerNum(); // partnerNum

    PlayerCardPair winningPair = gamedata.handInfo.getWinningPlayerCardPair(gamedata.roundInfo);
    winningCard = winningPair.card;
    winningPlayerNum = winningPair.playerNum;

    if (!winningCard.isUndefined())
    {
        winningSuitCardsLeft.append(gamedata.detailRoundInfo.suitCardsPlayed[winningCard.suit]
                                        .getOtherCardsThisSuit(winningCard.suit, gamedata.roundInfo.trump));

        if (!winningSuitCardsLeft.empty())
        {
            maxWinningSuitCardLeft = *max_element(winningSuitCardsLeft.begin(), winningSuitCardsLeft.end());
        }
    }

    trumpCardsLeft.append(gamedata.detailRoundInfo.suitCardsPlayed[gamedata.roundInfo.trump]
                              .getOtherCardsThisSuit(gamedata.roundInfo.trump, gamedata.roundInfo.trump));

    if (!trumpCardsLeft.empty())
    {
        maxTrumpCardLeft = *max_element(trumpCardsLeft.begin(), trumpCardsLeft.end());
    }

    initializeProbWinningCardTrumpable(); // probWinningCardTrumpable

    initializeCurrentHandCards(); // currentHandCards
}

bool CpuPlayCardStrategyInputs::isWinningPlayerNumLikelyToWinHand()
{
    if (winningCard >= maxWinningSuitCardLeft)
    {
        if (probWinningCardTrumpable < 0.5)
        {
            return true;
        }
    }

    return false;
}

bool CpuPlayCardStrategyInputs::isPartner(int playerNum)
{
    return (std::find(partnerNum.begin(), partnerNum.end(), playerNum) != partnerNum.end());
}

bool CpuPlayCardStrategyInputs::isHandWorthWinning()
{
    // worth winning if current hand has points OR
    // cpu can win hand with a 14
    return currentHandCards.getNumPoints() > 0 || playableCards.hasCard({winningCard.suit, VALUE_14});
}

bool CpuPlayCardStrategyInputs::isCpuAbleToWinHand()
{
    if (playableCards.hasSuit(winningCard.suit)) // follow suit
    {
        return playableCards.hasCard(maxWinningSuitCardLeft);
    }
    else // trump
    {
        return playableCards.hasSuit(gamedata.roundInfo.trump);
    }
}

Cpu::Cpu(const int &pPlayerNum, int &pTeamNum, CardVector &pCardArr, int &pBid, bool &pPassed) : playerNum(pPlayerNum),
                                                                                                 teamNum(pTeamNum),
                                                                                                 cardArr(pCardArr),
                                                                                                 bid(pBid),
                                                                                                 passed(pPassed)
{
}

void Cpu::clear()
{
    // do nothing
}

int Cpu::getBid() const
{
    cardArr.sort();

    double averageValue = (double)cardArr.getTotalValue() / cardArr.size();

    const double MU = 7.54;
    const double SIGMA = 0.987;
    double PHand = Utils::Stat::phi((averageValue - MU) / SIGMA); // percentile for hand

    int minBid = []() {
        switch (Settings::Game::readCpuBidAggressionLevel())
        {
        case CPU_BID_AGGRESSION_LEVEL_LOW:
            return 45;
        case CPU_BID_AGGRESSION_LEVEL_MID:
            return 50;
        default: // CPU_BID_AGGRESSION_LEVEL_HIGH
            return 55;
        }
    }();

    map<double, int> bidMap;

    for (double P = 0.1; P <= 0.9; P += 0.1)
    {
        bidMap[P] = minBid + (P * 10) * 5;
    }

    auto It = bidMap.upper_bound(PHand);
    auto maxBid = It != bidMap.end() ? It->second : (--bidMap.end())->second + 5;

    if (maxBid > gamedata.roundInfo.bidAmount)
    {
        return gamedata.roundInfo.bidAmount + 5;
    }
    else
    {
        return gamedata.playerArr[playerNum].bid; // bid unchanged
    }
}

// output: gamedata.nest, this->cardArr
void Cpu::selectNest()
{
    CardVector combinedCardArr;
    combinedCardArr.append(gamedata.nest);
    combinedCardArr.append(cardArr);

    // best cards at front, worst cards at back
    CardVector cardQualityQueue = combinedCardArr.getCardQualityQueue();

    CardVector newNest;
    CardVector newCardArr;

    const int numNestCardsAllowed = Settings::Game::readNumCardsMiddleAllowed();
    int numNestCardsTaken = 0;

    for (auto &card : cardQualityQueue)
    {
        // we should check that player's hand is not too large here before trying to take more cards
        if (newCardArr.size() == 13)
        {
            newNest.push_back(card);
            continue;
        }

        if (gamedata.nest.hasCard(card))
        {
            if (numNestCardsTaken < numNestCardsAllowed)
            {
                newCardArr.push_back(card);
                numNestCardsTaken++;
            }
            else
            {
                newNest.push_back(card);
            }
        }
        else
        {
            newCardArr.push_back(card);
        }
    }

    newCardArr.sort();
    newNest.sort();

    cardArr = newCardArr;
    gamedata.nest = newNest;
}

// output: gamedata.roundInfo.trump
void Cpu::selectTrump()
{
    auto suitInfoArr = cardArr.getSuitInfoArray();
    int bestSuit = suitInfoArr.front().suit;

    gamedata.roundInfo.trump = bestSuit;
}

// output: gamedata.roundInfo.partnerCard
void Cpu::selectPartner()
{
    CardVector otherCards; // don't pick own cards as partner

    for (auto suit : {SUIT_BLACK, SUIT_GREEN, SUIT_RED, SUIT_YELLOW})
    {
        otherCards.append(cardArr.getOtherCardsThisSuit(suit, SUIT_UNDEFINED));
    }

    otherCards.remove(gamedata.nest); // don't pick nest card as partner

    // logic from selectTrump (partner is conditional on trump suit)
    auto suitInfoArr = cardArr.getSuitInfoArray();
    int bestSuit = suitInfoArr.front().suit;

    CardVector bestCards = otherCards.getCardsThisSuit(bestSuit);
    bestCards.sort();

    Card bestCard = bestCards.back();
    gamedata.roundInfo.partnerCard = bestCard;
}

bool Cpu::useRandomStrategy() const
{
    int randomnessLevel = Settings::Game::readCpuRandomnessLevel();
    double p = ((double)rand() / (RAND_MAX));

    switch (randomnessLevel)
    {
    case CPU_RANDOMNESS_LEVEL_NONE:
        return false;
    case CPU_RANDOMNESS_LEVEL_LOW:
        return (p <= 0.1);
    case CPU_RANDOMNESS_LEVEL_MID:
        return (p <= 0.2);
    case CPU_RANDOMNESS_LEVEL_HIGH:
        return (p <= 0.3);
    }
}

PlayCardStrategy Cpu::getPlayCardStrategy(CpuPlayCardStrategyInputs &inputs) const
{
    if (useRandomStrategy())
    {
        return STRATEGY_RANDOM;
    }
    
    // deterministic strategy
    if (inputs.isPartner(inputs.winningPlayerNum))
    {
        if (inputs.isWinningPlayerNumLikelyToWinHand())
        {
            return STRATEGY_FEED_POINTS;
        }
        else
        {
            if (inputs.isHandWorthWinning())
            {
                if (inputs.isCpuAbleToWinHand())
                {
                    return STRATEGY_PLAY_HIGHEST_CARD;
                }
                else
                {
                    return STRATEGY_THROW_AWAY_CARD;
                }
            }
            else
            {
                return STRATEGY_THROW_AWAY_CARD;
            }
        }
    }
    else
    {
        if (inputs.isHandWorthWinning())
        {
            if (inputs.isCpuAbleToWinHand())
            {
                return STRATEGY_PLAY_HIGHEST_CARD;
            }
            else
            {
                return STRATEGY_THROW_AWAY_CARD;
            }
        }
        else
        {
            return STRATEGY_THROW_AWAY_CARD;
        }
    }

    return STRATEGY_UNDEFINED;
}

Card Cpu::getCardToPlay() const
{
    CpuPlayCardStrategyInputs inputs(playerNum);
    PlayCardStrategy strategy = getPlayCardStrategy(inputs);

    switch (strategy)
    {
    case PlayCardStrategy::STRATEGY_RANDOM:
        return getRandomCardToPlay(inputs);
    case PlayCardStrategy::STRATEGY_FEED_POINTS:
        return getFeedCardToPlay(inputs);
    case PlayCardStrategy::STRATEGY_PLAY_HIGHEST_CARD:
        return getHighestCardToPlay(inputs);
    case PlayCardStrategy::STRATEGY_THROW_AWAY_CARD:
        return getThrowAwayCardToPlay(inputs);
    default:
        return Card(SUIT_UNDEFINED, VALUE_UNDEFINED); // should not happen
    }
}

Card Cpu::getRandomCardToPlay(CpuPlayCardStrategyInputs &inputs) const
{
    auto &playableCards = inputs.playableCards;
    int index = Utils::Random::randbetween(0, (int)playableCards.size() - 1);
    Card cardToPlay = playableCards[index];
    return cardToPlay;
}

Card Cpu::getFeedCardToPlay(CpuPlayCardStrategyInputs &inputs) const
{
    auto &playableCards = inputs.playableCards;
    Card cardToPlay = playableCards.front();

    for (auto it = playableCards.begin() + 1; it != playableCards.end(); it++)
    {
        if (it->getPointValue() > cardToPlay.getPointValue())
        {
            cardToPlay = *it;
        }
    }

    return cardToPlay;
}

Card Cpu::getHighestCardToPlay(CpuPlayCardStrategyInputs &inputs) const
{
    auto &playableCards = inputs.playableCards;
    Card cardToPlay; // undefined

    if (playableCards.hasSuit(inputs.winningCard.suit)) // highest card of this suit
    {
        cardToPlay = *max_element(playableCards.begin(), playableCards.end());
    }
    else // lowest trump
    {
        CardVector trumpCards = playableCards.getCardsThisSuit(gamedata.roundInfo.trump);
        cardToPlay = *min_element(trumpCards.begin(), trumpCards.end());
    }

    return cardToPlay;
}

Card Cpu::getThrowAwayCardToPlay(CpuPlayCardStrategyInputs &inputs) const
{
    auto card_is_trump = [](const Card &card) { return card.suit == gamedata.roundInfo.trump; };
    auto card_has_points = [](const Card &card) { return card.getPointValue() > 0; };

    auto &playableCards = inputs.playableCards;
    Card cardToPlay; // undefined

    CardVector throwAwayCards = playableCards;
    throwAwayCards.erase(remove_if(throwAwayCards.begin(), throwAwayCards.end(), card_is_trump), throwAwayCards.end());

    if (throwAwayCards.empty()) // cpu only has trump to play
    {
        throwAwayCards = playableCards;
        throwAwayCards.erase(remove_if(throwAwayCards.begin(), throwAwayCards.end(), card_has_points), throwAwayCards.end());

        if (throwAwayCards.empty()) // cpu only has trump with points
        {
            cardToPlay = *min_element(playableCards.begin(), playableCards.end());
        }
        else
        {
            cardToPlay = *min_element(throwAwayCards.begin(), throwAwayCards.end());
        }
    }
    else
    {
        throwAwayCards.erase(remove_if(throwAwayCards.begin(), throwAwayCards.end(), card_has_points), throwAwayCards.end());

        if (throwAwayCards.empty()) // cpu only has cards with points
        {
            cardToPlay = *min_element(playableCards.begin(), playableCards.end());
        }
        else // prioritize clearing out colors
        {
            map<int, int> suitfreq;
            for (auto &card : throwAwayCards)
            {
                suitfreq[card.suit]++;
            }

            auto it = min_element(suitfreq.begin(), suitfreq.end(),
                                  [](const pair<int, int> &left, const pair<int, int> &right) { return left.second < right.second; });

            int suitToPlay = it->first;

            throwAwayCards = throwAwayCards.getCardsThisSuit(suitToPlay);
            cardToPlay = *min_element(throwAwayCards.begin(), throwAwayCards.end());
        }
    }

    return cardToPlay;
}