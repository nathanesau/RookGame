#include "cpuPlayer.h"
#include "gameData.h"
#include "utils.h"

#include <map>

using namespace std;

CpuPlayer::CpuPlayer()
{
}

int CpuPlayer::getBid(int playerNum)
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;
    cardArr.sort();

    double totalValue = 0.0;

    for (auto &card : cardArr)
    {
        totalValue += card.value;
    }

    double averageValue = totalValue / cardArr.size();

    const double MU = 7.54;
    const double SIGMA = 0.987;

    double P = Utils::Stat::phi((averageValue - MU) / SIGMA); // percentile

    map<double, int> bidMap{
        make_pair(0.1, 50), make_pair(0.2, 55), make_pair(0.3, 60), make_pair(0.4, 65),
        make_pair(0.5, 70), make_pair(0.6, 75), make_pair(0.7, 80), make_pair(0.8, 85),
        make_pair(0.9, 90)};

    auto It = bidMap.upper_bound(P);
    auto maxBid = It != bidMap.end() ? It->second : 95;

    int bid = maxBid > gamedata.roundInfo.bidAmount ? gamedata.roundInfo.bidAmount + 5 : gamedata.playerArr[playerNum].bid;

    return bid;
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

Card CpuPlayer::getCardToPlay(int playerNum) // to be improved
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;

    CardVector playableCards = cardArr.getPlayableCards(gamedata.handInfo);
    playableCards.sort(gamedata.roundInfo.trump);

    // determine which card to play
    int winningPlayerNum = gamedata.handInfo.getWinningPlayerNum(gamedata.roundInfo);
    int partnerNum = getPartnerNum(playerNum);

    if (winningPlayerNum == partnerNum) // "feed" points
    {
        return playableCards.getCardWithHighestPointValue();
    }
    else
    {
        // check if their are points in the current hand
        if (gamedata.handInfo.points > 0)
        {
            auto highestCard = *--playableCards.end();
            auto winningCard = gamedata.handInfo.getWinningCard(gamedata.roundInfo);
            
            CardCompare cardCompare(gamedata.roundInfo.trump);
            
            if (cardCompare(winningCard, highestCard)) // try to win hand (since it has points and is winnable)
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

CardVector CpuPlayer::getChosenNest(int playerNum)
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

int CpuPlayer::getChosenTrump(int playerNum)
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;

    vector<SuitInfo> suitInfoArr = cardArr.getSuitInfoArray();

    return suitInfoArr[0].suit != SUIT_SPECIAL ? suitInfoArr[0].suit : suitInfoArr[1].suit;
}

Card CpuPlayer::getChosenPartner(int playerNum)
{
    CardVector &cardArr = gamedata.playerArr[playerNum].cardArr;
    vector<const CardVector *> cardArrays = {&gamedata.nest};

    for(auto thisPlayerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if(thisPlayerNum != playerNum)
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