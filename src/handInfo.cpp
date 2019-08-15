#include <vector>

#include "handInfo.h"
#include "roundInfo.h"

using namespace std;

HandInfo::HandInfo()
{
    clear();
}

void HandInfo::clear()
{
    winningCard = Card(SUIT_UNDEFINED, VALUE_UNDEFINED);
    winningPlayerNum = PLAYER_UNDEFINED;
    startingPlayerNum = PLAYER_UNDEFINED;
    cardPlayed.clear();
    points = 0;
    suit = SUIT_UNDEFINED;
}

int HandInfo::getWinningPlayerNum(const RoundInfo &roundInfo)
{
    updateWinningCombination(roundInfo);

    return winningPlayerNum;
}

Card HandInfo::getWinningCard(const RoundInfo &roundInfo)
{
    updateWinningCombination(roundInfo);

    return winningCard;
}

void HandInfo::updateWinningCombination(const RoundInfo &roundInfo)
{
    winningPlayerNum = startingPlayerNum;
    winningCard = cardPlayed[startingPlayerNum];

    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        Card currentCard = cardPlayed[playerNum];

        if (playerNum == winningPlayerNum)
            continue;

        if (currentCard == Card(SUIT_UNDEFINED, VALUE_UNDEFINED))
            continue;

        if (currentCard.suit == winningCard.suit)
        {
            if (currentCard.value > winningCard.value)
            {
                winningPlayerNum = playerNum;
                winningCard = currentCard;
            }
        }
        else // not the same suit
        {
            if (currentCard.suit == roundInfo.trump)
            {
                winningPlayerNum = playerNum;
                winningCard = currentCard;
            }
        }
    }
}
