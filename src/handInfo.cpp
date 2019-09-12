#include "handInfo.h"
#include "roundInfo.h"

HandInfo::HandInfo()
{
    clear();
}

void HandInfo::clear()
{
    startingPlayerNum = PLAYER_UNDEFINED;
    cardPlayed.clear();
    points = 0;
    suit = SUIT_UNDEFINED;
}

PlayerCardPair HandInfo::getWinningPlayerCardPair(const RoundInfo &roundInfo) const
{
    auto getCardPlayed = [this](int playerNum)
    {
        auto it = cardPlayed.find(playerNum);
        return (it != cardPlayed.end()) ? it->second : Card(SUIT_UNDEFINED, VALUE_UNDEFINED);
    };

    PlayerCardPair winningPair(startingPlayerNum, getCardPlayed(startingPlayerNum));

    for (auto playerNum : std::vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        Card currentCard = getCardPlayed(playerNum);

        if (playerNum == winningPair.playerNum)
            continue;

        if (currentCard.isUndefined())
            continue;

        if (currentCard.suit == winningPair.card.suit)
        {
            if (currentCard.value > winningPair.card.value)
            {
                winningPair.playerNum = playerNum;
                winningPair.card = currentCard;
            }
        }
        else // not the same suit
        {
            if (currentCard.suit == roundInfo.trump)
            {
                winningPair.playerNum = playerNum;
                winningPair.card = currentCard;
            }
        }
    }
    
    return winningPair;
}
