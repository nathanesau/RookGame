#include <algorithm>
#include <array>
#include <chrono>
#include <random>
#include <vector>

#include "card.h"
#include "deck.h"
#include "player.h"

using namespace std;

Deck::Deck()
{
    seed = (int)chrono::system_clock::now().time_since_epoch().count();
}

#ifdef QT_DEBUG
void Deck::setSeed(int pSeed)
{
    seed = pSeed;
}
#endif

void Deck::initialize()
{
    for (auto suit : vector<int>{SUIT_BLACK, SUIT_GREEN, SUIT_RED, SUIT_YELLOW})
    {
        for (auto value : vector<int>{VALUE_1, VALUE_2, VALUE_3, VALUE_4,
                                      VALUE_5, VALUE_6, VALUE_7, VALUE_8,
                                      VALUE_9, VALUE_10, VALUE_11, VALUE_12,
                                      VALUE_13, VALUE_14})
        {
            cardArr.push_back(Card(suit, value));
        }
    }

    cardArr.push_back(Card(SUIT_SPECIAL, VALUE_ROOK));

    shuffle(cardArr.begin(), cardArr.end(), default_random_engine(seed));
}

void Deck::deal(array<Player, 4> &playerArr, CardVector &nest)
{
    int playerNum = PLAYER_1;

    while (cardArr.size() > 5)
    {
        Card currentCard = cardArr.back(); // copy
        cardArr.pop_back();

        playerArr[playerNum].cardArr.push_back(currentCard);
        playerNum = playerArr[playerNum].getNextPlayerNum();
    }

    while (cardArr.size() > 0)
    {
        Card currentCard = cardArr.back(); // copy
        cardArr.pop_back();

        nest.push_back(currentCard);
    }

    nest.sort();
    playerArr[PLAYER_1].cardArr.sort();
    #ifdef CPU_DEBUG
    playerArr[PLAYER_2].cardArr.sort();
    playerArr[PLAYER_3].cardArr.sort();
    playerArr[PLAYER_4].cardArr.sort();
    #endif
}
