#include <QSettings>

#include "cpuPlayer.h"
#include "deck.h"
#include "gameController.h"
#include "utils.h"

GameController::GameController()
{
}

void GameController::startNewRound()
{
    data.clearRoundSpecificInfo();
    data.overallInfo.roundNum += 1;

    Deck deck;
    deck.initialize();
    deck.deal(data.playerArr, data.nest);
}

void GameController::getCpuBids()
{
    for (auto playerNum : vector<int>{PLAYER_2, PLAYER_3, PLAYER_4})
    {
        auto &player = data.playerArr[playerNum];

        if (player.passed)
            continue;

        if (player.bid == data.roundInfo.bidAmount)
            continue;

        int prevBid = player.bid;
        player.bid = cpu.getBid(playerNum);
        player.passed = (player.bid == prevBid);

        if (player.bid > data.roundInfo.bidAmount)
        {
            data.roundInfo.bidAmount = data.playerArr[playerNum].bid;
            data.roundInfo.bidPlayer = playerNum;
        }
    }
}

int GameController::getNumPassed()
{
    return accumulate(data.playerArr.begin(), data.playerArr.end(), 0, [&](int a, Player &b) { return a + (b.passed); });
}

void GameController::setRookSuitToTrump()
{
    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        for (auto &card : gc.data.playerArr[playerNum].cardArr)
        {
            if (card.suit == SUIT_SPECIAL)
            {
                card.suit = gc.data.roundInfo.trump;
                return;
            }
        }
    }    
}

void GameController::playCard(Card cardPlayed, int playerNum)
{
    if (data.handInfo.startingPlayerNum == PLAYER_UNDEFINED) // first card played
    {
        data.handInfo.startingPlayerNum = playerNum;
        data.handInfo.suit = cardPlayed.suit;
    }

    data.handInfo.cardPlayed[playerNum] = cardPlayed;
    data.handInfo.points += cardPlayed.getPointValue();

    if (cardPlayed == data.roundInfo.partnerCard) // update partner and teams
    {
        data.roundInfo.partnerPlayerNum = playerNum;
        data.roundInfo.updateTeams();
    }

    data.playerArr[playerNum].cardArr.remove({cardPlayed});
}

bool GameController::isRoundOver()
{
    return std::all_of(data.playerArr.begin(), data.playerArr.end(), [](auto &p) { return p.cardArr.empty(); });
}