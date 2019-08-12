#include "cpuPlayer.h"
#include "gameController.h"
#include "utils.h"

#include <map>

using namespace std;

CpuPlayer::CpuPlayer()
{
}

int CpuPlayer::getBid(int playerNum)
{
    CardVector &cardArr = gc.data.playerArr[playerNum].cardArr;
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

    int bid = maxBid > gc.data.roundInfo.bidAmount ? gc.data.roundInfo.bidAmount + 5 : gc.data.playerArr[playerNum].bid;

    return bid;
}

Card CpuPlayer::getCardToPlay(int playerNum) // to be improved
{
    CardVector &cardArr = gc.data.playerArr[playerNum].cardArr;

    CardVector playableCards = cardArr.getPlayableCards(gc.data.handInfo);

    playableCards.sort(gc.data.roundInfo.trump);
   
    auto It = --playableCards.end();
    return *It;
}

CardVector CpuPlayer::getChosenNest(int playerNum)
{
    CardVector &cardArr = gc.data.playerArr[playerNum].cardArr;
    
    CardVector newNest;
    CardVector newCardArr;
    newCardArr.append({&cardArr, &gc.data.nest});

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
    CardVector &cardArr = gc.data.playerArr[playerNum].cardArr;

    vector<SuitInfo> suitInfoArr = cardArr.getSuitInfoArray();

    return suitInfoArr[0].suit != SUIT_SPECIAL ? suitInfoArr[0].suit : suitInfoArr[1].suit;
}

Card CpuPlayer::getChosenPartner(int playerNum)
{
    CardVector &cardArr = gc.data.playerArr[playerNum].cardArr;
    vector<const CardVector *> cardArrays = {&gc.data.nest};

    for(auto thisPlayerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if(thisPlayerNum != playerNum)
        {
            cardArrays.push_back(&gc.data.playerArr[thisPlayerNum].cardArr);   
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