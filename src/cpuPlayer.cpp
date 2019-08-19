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

CardVector CpuDecisionMaker::getChosenNest(int playerNum) const
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;

    CardVector newNest;
    CardVector newCardArr;
    newCardArr.append(cardArr);
    newCardArr.append(gamedata.nest);

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

    pair<int, int> otherCpuPlayers = [](int playerNum) {
        switch (playerNum)
        {
        case PLAYER_2:
            return make_pair(PLAYER_3, PLAYER_4);
        case PLAYER_3:
            return make_pair(PLAYER_2, PLAYER_4);
        default: // PLAYER_4
            return make_pair(PLAYER_2, PLAYER_3);
        }
    }(playerNum);

    CardVector aggregateCardArr;
    aggregateCardArr.append(gamedata.nest);
    aggregateCardArr.append(gamedata.playerArr[PLAYER_1].cardArr);
    aggregateCardArr.append(gamedata.playerArr[otherCpuPlayers.first].cardArr);
    aggregateCardArr.append(gamedata.playerArr[otherCpuPlayers.second].cardArr);

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

int CpuDecisionMaker::getPartnerNum(int playerNum) const
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
                std::copy_if(team.begin(), team.end(), std::back_inserter(teamCpy), [playerNum](auto p) { return p != playerNum; });
                return teamCpy.front();
            }
        }
    }

    return PLAYER_UNDEFINED;
}

Card CpuDecisionMaker::getCardToPlay(int playerNum) const
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;

    // information used to make decision
    CardVector playableCards = cardArr.getPlayableCards(gamedata.handInfo);
    PlayerCardPair winningPair = gamedata.handInfo.getWinningPlayerCardPair(gamedata.roundInfo);
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