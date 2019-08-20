#ifndef CARD_H
#define CARD_H

#include <array>
#include <string>
#include <vector>

#include "common.h"

using namespace std;

// DO NOT MODIFY
const int SUIT_UNDEFINED = -1;
const int SUIT_BLACK = 0;
const int SUIT_GREEN = 1;
const int SUIT_RED = 2;
const int SUIT_YELLOW = 3;
const int SUIT_SPECIAL = 4;

// DO NOT MODIFY
const int VALUE_UNDEFINED = -1;
const int VALUE_ROOK = 0;
const int VALUE_1 = 1;
const int VALUE_2 = 2;
const int VALUE_3 = 3;
const int VALUE_4 = 4;
const int VALUE_5 = 5;
const int VALUE_6 = 6;
const int VALUE_7 = 7;
const int VALUE_8 = 8;
const int VALUE_9 = 9;
const int VALUE_10 = 10;
const int VALUE_11 = 11;
const int VALUE_12 = 12;
const int VALUE_13 = 13;
const int VALUE_14 = 14;

// forward declarations
class HandInfo;

struct Card
{
    int suit;
    int value;

    Card(int pSuit = SUIT_UNDEFINED, int pValue = VALUE_UNDEFINED);
    bool operator==(const Card &p);
    bool operator!=(const Card &p);
    bool operator<(const Card &p);

    bool isUndefined() const
    {
        return suit == SUIT_UNDEFINED && value == VALUE_UNDEFINED;
    }

    int getPointValue() const;
    int getSentimentalValue() const;

    string getSuitAsString() const; // for filenames
    string getValueAsString() const; // for filenames

    string getCardAsString() const; // for save slot display
};

struct CardCompare
{
    int pTrump;

    CardCompare(int trump) : pTrump(trump)
    {
    }

    inline bool operator()(const Card &card1, const Card &card2)
    {
        if (pTrump != SUIT_UNDEFINED && card1.suit == pTrump && card2.suit != pTrump)
        {
            return false;
        }
        else if (card1.suit < card2.suit)
        {
            return true;
        }
        else if (card1.suit == card2.suit)
        {
            if (card1.value < card2.value)
            {
                return true;
            }
        }

        return false;
    }
};

struct SuitInfo
{
    int suit;
    int count;
    int totalValue;

    SuitInfo(int pSuit, int pCount = 0, int pTotalValue = 0) : suit(pSuit), count(pCount), totalValue(pTotalValue)
    {
    }

    bool operator==(const SuitInfo &p)
    {
        return p.suit == suit;
    }
};

struct SuitInfoCompareCount // ascending order
{
    inline bool operator()(const SuitInfo &info1, const SuitInfo &info2)
    {
        return info1.count > info2.count;
    }
};

struct SuitInfoCompareTotalValue // ascending order
{
    inline bool operator()(const SuitInfo &info1, const SuitInfo &info2)
    {
        return info1.totalValue > info2.totalValue;
    }
};

using SuitInfoArray = vector<SuitInfo>;

// do not delete objects through base class pointer
// https://stackoverflow.com/questions/56296536/how-to-derive-from-vector-class
class CardVector : public vector<Card>
{
    using vector<Card>::vector;
    using CardVectorRef = CardVector &;

public:
    void sort(int trump = SUIT_UNDEFINED);
    void remove(const CardVector &cardArr);
    void append(const CardVector &cardArr);

    bool hasSuit(int suit) const;
    bool hasCard(const Card &card) const;

    int getNumPoints() const;
    int getTotalValue() const;

    CardVector getCardsThisSuit(int suit) const;
    CardVector getPlayableCards(const HandInfo &handInfo) const;
    CardVector getCardQualityQueue() const;
    SuitInfoArray getSuitInfoArray() const;

    Card getCardWithHighestPointValue() const;
    Card getCardWithLowestPointValue() const; 
};

using CardVectorRef = CardVector &;

#endif