#include <algorithm>
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

int Card::getPointValue()
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

int Card::getSentimentalValue()
{
    if (value == VALUE_ROOK)
    {
        return 100;
    }

    return value;
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
        if(!s.empty())
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

void CardVector::sort(int trump)
{
    std::sort(this->begin(), this->end(), CardCompare(trump));
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

CardVector CardVector::removeThisSuit(int suit, int n)
{
    sort();

    CardVector cardsRemoved; // return cards which were removed

    if (n > 0)
    {
        for (auto It = this->begin(); It != this->end();)
        {
            if (It->suit == suit)
            {
                cardsRemoved.push_back(*It);
                It = this->erase(It);
            }
            else
            {
                It++;
            }

            if (cardsRemoved.size() == n)
            {
                break;
            }
        }
    }

    return cardsRemoved;
}

void CardVector::append(const vector<const CardVector *> &cardArrays)
{
    for (auto cardArr : cardArrays)
    {
        if (cardArr)
        {
            for (auto It = cardArr->begin(); It != cardArr->end(); It++)
            {
                this->push_back(*It);
            }
        }
    }
}

bool CardVector::hasSuit(int suit)
{
    return std::any_of(begin(), end(), [suit](Card &a) { return a.suit == suit; });
}

bool CardVector::hasCard(const Card &card)
{
    return std::find_if(begin(), end(), [a = card](Card &b) { return a.suit == b.suit && a.value == b.value; }) != end();
}

int CardVector::getNumPoints()
{
    return accumulate(begin(), end(), 0, [](int a, Card &b) { return a + b.getPointValue(); });
}

CardVector CardVector::getPlayableCards(const HandInfo &handInfo)
{
    int suit = handInfo.suit;

    if (hasSuit(suit))
    {
        CardVector playableCards;

        for (auto It = this->begin(); It != this->end(); It++)
        {
            if (It->suit == suit)
            {
                playableCards.push_back(*It);
            }
        }

        return playableCards;
    }
    else // all cards are playable
    {
        return *this;
    }
}

vector<SuitInfo> CardVector::getSuitInfoArray()
{
    vector<SuitInfo> suitInfoArr;

    for (auto suit : vector<int>{SUIT_BLACK, SUIT_GREEN, SUIT_RED, SUIT_YELLOW, SUIT_SPECIAL})
    {
        suitInfoArr.push_back(SuitInfo(suit));
    }

    for (auto It = this->begin(); It != this->end(); It++)
    {
        auto SuitIt = std::find(suitInfoArr.begin(), suitInfoArr.end(), SuitInfo(It->suit));

        SuitIt->count += 1;
        SuitIt->totalValue += It->getSentimentalValue();
    }

    std::sort(suitInfoArr.begin(), suitInfoArr.end(), SuitInfoCompareCount());
    std::sort(suitInfoArr.begin(), suitInfoArr.end(), SuitInfoCompareTotalValue());

    return suitInfoArr;
}