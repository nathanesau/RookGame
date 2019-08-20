#include "cpu.h"
#include "gameData.h"
#include "settings.h"
#include "utils.h"

#include <map>

using namespace std;

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

    double averageValue = (double) cardArr.getTotalValue() / cardArr.size();

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
    SuitInfoArray suitInfoArr = cardArr.getSuitInfoArray();
    std::remove_if(suitInfoArr.begin(), suitInfoArr.end(), [](const SuitInfo &info) { return info.suit == SUIT_SPECIAL; });

    int bestSuit = suitInfoArr.front().suit;
    gamedata.roundInfo.trump = bestSuit;
}

// output: gamedata.roundInfo.partnerCard
void Cpu::selectPartner()
{
    CardVector aggregateCardArr;
    aggregateCardArr.append(gamedata.nest);

    for(auto otherPlayerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if (playerNum != otherPlayerNum)
        {
            aggregateCardArr.append(gamedata.playerArr[otherPlayerNum].cardArr);
        }
    }

    auto suitInfoArr = cardArr.getSuitInfoArray();
    std::remove_if(suitInfoArr.begin(), suitInfoArr.end(), [](const SuitInfo &info) { return info.suit == SUIT_SPECIAL; });

    int bestSuit = suitInfoArr.front().suit;
    Card bestCard(bestSuit, VALUE_1);

    // guaranteed to find at least one card of bestSuit
    for (auto &card : aggregateCardArr)
    {
        if (card.suit != bestSuit)
            continue;

        if (card.value > bestCard.value)
        {
            bestCard = card;
        }
    }

    gamedata.roundInfo.partnerCard = bestCard;
}

int Cpu::getPartnerNum() const
{
    if (gamedata.playerArr[playerNum].cardArr.hasCard(gamedata.roundInfo.partnerCard))
    {
        return gamedata.roundInfo.bidPlayer;
    }
    else
    {
        for (auto &team : gamedata.roundInfo.teams)
        {
            if (std::find(team.begin(), team.end(), playerNum) != team.end())
            {
                vector<int> teamCpy;
                std::copy_if(team.begin(), team.end(), std::back_inserter(teamCpy), [this](auto p) { return p != playerNum; });
                return teamCpy.front();
            }
        }
    }

    return PLAYER_UNDEFINED;
}

Card Cpu::getCardToPlay() const
{
    // information used to make decision
    CardVector playableCards = cardArr.getPlayableCards(gamedata.handInfo);
    PlayerCardPair winningPair = gamedata.handInfo.getWinningPlayerCardPair(gamedata.roundInfo);
    int partnerNum = getPartnerNum();

    if (winningPair.playerNum == partnerNum) // "feed" points
    {
        return playableCards.getCardWithHighestPointValue();
    }
    else
    {
        // check if their are points in the current hand
        if (gamedata.handInfo.points > 0)
        {
            auto highestCard = *--playableCards.end();

            CardCompare cardCompare(gamedata.roundInfo.trump);

            if (cardCompare(winningPair.card, highestCard)) // try to win hand (since it has points and is winnable)
            {
                return highestCard;
            }
        }
        else // leave the hand, but don't throw points
        {
            return playableCards.getCardWithLowestPointValue();
        }
    }

    return playableCards.getCardWithLowestPointValue(); // save best cards for last
}