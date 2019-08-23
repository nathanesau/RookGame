#include <algorithm>
#include <array>
#include <map>
#include <numeric>

#include "card.h"
#include "handInfo.h"

using namespace std;

Card::Card(int pSuit, int pValue) : suit(pSuit), value(pValue)
{
}

bool Card::operator==(const Card &p)
{
    return p.value == value && p.suit == suit;
}

bool Card::operator!=(const Card &p)
{
    return p.value != value || p.suit != suit;
}

bool Card::operator<(const Card &p)
{
    if (suit < p.suit)
    {
        return true;
    }
    else if (suit == p.suit)
    {
        if (value < p.value)
        {
            return true;
        }
    }

    return false;
}

bool Card::operator>=(const Card &p)
{
    if(*this == p) return true;
    else return !(*this < p);
}

int Card::getPointValue() const
{
    switch (value)
    {
    case VALUE_10:
    case VALUE_14:
        return 10;
    case VALUE_ROOK:
        return 20;
    case VALUE_5:
        return 5;
    default:
        return 0;
    }
}

string Card::getSuitAsString() const
{
    if (value == VALUE_ROOK) // rook suit can change
    {
        return "";
    }

    switch (suit)
    {
    case SUIT_BLACK:
        return "black";
    case SUIT_GREEN:
        return "green";
    case SUIT_RED:
        return "red";
    case SUIT_YELLOW:
        return "yellow";
    case SUIT_SPECIAL:
        return "";
    default: // SUIT_UNDEFINED
        return "blank";
    }
}

string Card::getValueAsString() const
{
    switch (value)
    {
    case VALUE_ROOK:
        return "rook";
    case VALUE_1:
        return "1";
    case VALUE_2:
        return "2";
    case VALUE_3:
        return "3";
    case VALUE_4:
        return "4";
    case VALUE_5:
        return "5";
    case VALUE_6:
        return "6";
    case VALUE_7:
        return "7";
    case VALUE_8:
        return "8";
    case VALUE_9:
        return "9";
    case VALUE_10:
        return "10";
    case VALUE_11:
        return "11";
    case VALUE_12:
        return "12";
    case VALUE_13:
        return "13";
    case VALUE_14:
        return "14";
    default: // VALUE_UNDEFINED
        return "";
    }
}

string Card::getCardAsString() const
{
    if (suit == SUIT_UNDEFINED || value == VALUE_UNDEFINED)
    {
        return "???";
    }

    // capitalize first letter
    auto toUpper = [](string &s) -> void {
        if (!s.empty())
        {
            s[0] = toupper(s[0]);
        }
    };

    string suitStr = getSuitAsString();
    toUpper(suitStr);

    string valueStr = getValueAsString();
    toUpper(valueStr);

    return valueStr + (!valueStr.empty() ? " " : "") + suitStr;
}

void CardVector::sort()
{
    std::sort(this->begin(), this->end(), CardCompare(SUIT_UNDEFINED));
}

void CardVector::append(const CardVector &cardArr)
{
    insert(end(), cardArr.begin(), cardArr.end());
}

void CardVector::remove(const CardVector &cardArr)
{
    for (int i = 0; i < cardArr.size(); i++)
    {
        auto card = cardArr[i];

        auto It = std::find(this->begin(), this->end(), card);

        if (It != this->end())
        {
            this->erase(It);
        }
    }
}

bool CardVector::hasSuit(int suit) const
{
    return std::any_of(begin(), end(), [suit](const Card &a) { return a.suit == suit; });
}

bool CardVector::hasCard(const Card &card) const
{
    return std::find_if(begin(), end(), [a = card](const Card &b) { return a.suit == b.suit && a.value == b.value; }) != end();
}

int CardVector::getNumPoints() const
{
    return accumulate(begin(), end(), 0, [](int a, const Card &b) { return a + b.getPointValue(); });
}

int CardVector::getTotalValue() const
{
    return accumulate(begin(), end(), 0, [](int a, const Card &b) { return a + b.value; });
}

CardVector CardVector::getCardsThisSuit(int suit) const
{
    CardVector cts;
    copy_if(begin(), end(), back_inserter(cts), [&](const Card &c) { return c.suit == suit; });
    return cts;
}

CardVector CardVector::getOtherCardsThisSuit(int suit, int trump) const
{
    CardVector otherSuitCards;

    for (auto value : {VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6, VALUE_7,
                       VALUE_8, VALUE_9, VALUE_10, VALUE_11, VALUE_12, VALUE_13, VALUE_14})
    {
        otherSuitCards.push_back({suit, value});
    }

    if (suit == trump)
    {
        otherSuitCards.push_back({suit, VALUE_ROOK});
    }

    otherSuitCards.remove(getCardsThisSuit(suit));

    return otherSuitCards;
}

CardVector CardVector::getPlayableCards(const HandInfo &handInfo) const
{
    int suit = handInfo.suit;

    if (hasSuit(suit))
    {
        CardVector playableCards;
        copy_if(begin(), end(), back_inserter(playableCards), [suit](auto &c) { return c.suit == suit; });
        return playableCards;
    }
    else // all cards are playable
    {
        return *this;
    }
}

CardVector CardVector::getCardQualityQueue() const
{
    // best suits are at front of suitInfoArr
    auto suitInfoArr = getSuitInfoArray();

    // from best cards (at front) to worst cards (at back)
    CardVector cardQualityQueue;

    for (auto It = suitInfoArr.begin(); It != suitInfoArr.end(); It++)
    {
        auto cardsThisSuit = getCardsThisSuit(It->suit);
        reverse(cardsThisSuit.begin(), cardsThisSuit.end());
        cardQualityQueue.append(cardsThisSuit);
    }

    // due to the way cardQualityQueue is constructed, if it contains rook, rook will be at back
    // move rook to front of queue in this case

    Card rook(SUIT_SPECIAL, VALUE_ROOK);

    if(cardQualityQueue.hasCard(rook))
    {
        cardQualityQueue.remove({rook});
        cardQualityQueue.insert(cardQualityQueue.begin(), rook);
    }

    return cardQualityQueue;
}

array<SuitInfo, 5> CardVector::getSuitInfoArray() const
{
    array<SuitInfo, 5> arr = {SuitInfo(SUIT_BLACK), SuitInfo(SUIT_GREEN), SuitInfo(SUIT_RED),
                              SuitInfo(SUIT_YELLOW), SuitInfo(SUIT_SPECIAL)};

    for (auto it = this->begin(); it != this->end(); it++)
    {
        arr[it->suit].count += 1;
        arr[it->suit].totalValue += it->value;
    }

    std::sort(arr.begin(), arr.end(), SuitInfoCompareCount());
    std::sort(arr.begin(), arr.end(), SuitInfoCompareTotalValue());

    return arr;
}

Card CardVector::getCardWithHighestPointValue() const
{
    Card highestPointCard = front();

    for (auto it = this->begin() + 1; it != this->end(); it++)
    {
        if (it->getPointValue() > highestPointCard.getPointValue())
        {
            highestPointCard = *it;
        }
    }

    return highestPointCard;
}

Card CardVector::getCardWithLowestPointValue() const
{
    // relies on cards being sorted from lowest to highest
    Card lowestPointCard = front();

    for (auto it = this->begin() + 1; it != this->end(); it++)
    {
        if (it->getPointValue() < lowestPointCard.getPointValue())
        {
            lowestPointCard = *it;
        }
    }

    return lowestPointCard;
}
