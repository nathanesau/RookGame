#include "cpuPlayer.h"
#include "gameData.h"
#include "settings.h"
#include "utils.h"

#include <map>

using namespace std;

CpuDecisionMaker::CpuDecisionMaker()
{
}

int CpuDecisionMaker::getBid(int playerNum) const
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;
    cardArr.sort();

    double totalValue = accumulate(cardArr.begin(), cardArr.end(), 0, [](int a, const Card &b) { return a + b.value; });
    double averageValue = totalValue / cardArr.size();

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
    
    for(double P = 0.1; P <= 0.9; P += 0.1)
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

// for bidding team, this would be their partner
// for opposite team, this would be the other player not on bidding team
int getPartnerNum(int playerNum)
{
    bool playerNumHasPartnerCard = gamedata.playerArr[playerNum].cardArr.hasCard(gamedata.roundInfo.partnerCard);

    if (playerNumHasPartnerCard) // the bidding player their partner
    {
        return gamedata.roundInfo.bidPlayer;
    }
    else // check if the teams are common knowledge yet
    {
        int playerNumTeamNum = gamedata.playerArr[playerNum].getTeamNum();

        if (playerNumTeamNum != TEAM_UNDEFINED)
        {
            auto &team = gamedata.roundInfo.teams[playerNumTeamNum];

            for (auto teamPlayerNum : team)
            {
                if (playerNum != teamPlayerNum) // found teammate
                {
                    return teamPlayerNum;
                }
            }
        }
    }

    return PLAYER_UNDEFINED;
}

Card CpuDecisionMaker::getCardToPlay(int playerNum) const // to be improved
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;

    CardVector playableCards = cardArr.getPlayableCards(gamedata.handInfo);
    playableCards.sort(gamedata.roundInfo.trump);

    PlayerCardPair winningPair = gamedata.handInfo.getWinningPlayerCardPair(gamedata.roundInfo);

    // determine which card to play
    int partnerNum = getPartnerNum(playerNum);

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

CardVector CpuDecisionMaker::getChosenNest(int playerNum) const
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;

    CardVector newNest;
    CardVector newCardArr;
    newCardArr.append({&cardArr, &gamedata.nest});

    auto suitInfoArr = cardArr.getSuitInfoArray();

    int cardsToRemove = 5;

    while (cardsToRemove > 0) // discard worst cards to nest
    {
        auto It = --suitInfoArr.end();

        if (It->count > 0)
        {
            int n = min(It->count, cardsToRemove);
            auto cardsRemoved = newCardArr.removeThisSuit(It->suit, n);
            cardsToRemove -= n;

            for (auto &card : cardsRemoved)
            {
                newNest.push_back(card);
            }
        }

        suitInfoArr.erase(It);
    }

    return newNest;
}

int CpuDecisionMaker::getChosenTrump(int playerNum) const
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;

    vector<SuitInfo> suitInfoArr = cardArr.getSuitInfoArray();

    return suitInfoArr[0].suit != SUIT_SPECIAL ? suitInfoArr[0].suit : suitInfoArr[1].suit;
}

Card CpuDecisionMaker::getChosenPartner(int playerNum) const
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;
    vector<const CardVector *> cardArrays = {&gamedata.nest};

    for (auto thisPlayerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if (thisPlayerNum != playerNum)
        {
            cardArrays.push_back(&gamedata.playerArr[thisPlayerNum].cardArr);
        }
    }

    CardVector aggregateCardArr;
    aggregateCardArr.append(cardArrays);
    aggregateCardArr.sort();

    auto suitInfoArr = cardArr.getSuitInfoArray();

    int bestSuit = suitInfoArr[0].suit != SUIT_SPECIAL ? suitInfoArr[0].suit : suitInfoArr[1].suit;

    Card bestCard(bestSuit, VALUE_1);

    for (auto &card : aggregateCardArr) // guaranteed to have at least one card of bestSuit
    {
        if (card.suit != bestSuit)
            continue;

        if (card.value > bestCard.value)
        {
            bestCard = card;
        }
    }

    assert(bestCard != Card(bestSuit, VALUE_1)); // found appropriate partner card

    return bestCard;
}