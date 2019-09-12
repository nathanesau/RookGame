#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include "common.h"
#include "player.h"

// forward declarations
struct GameData;

// global declarations
extern GameData gamedata;

struct PlayerCardsTableRow
{
    int Player;
    int Suit;
    int Value;
    PlayerCardsTableRow(int pPlayer, int pSuit, int pValue) : Player(pPlayer), Suit(pSuit), Value(pValue)
    {
    }
};

struct PlayerBidsTableRow
{
    int Player;
    int Bid;
    PlayerBidsTableRow(int pPlayer, int pBid) : Player(pPlayer), Bid(pBid)
    {
    }
};

struct NestCardsTableRow
{
    int Suit;
    int Value;
    NestCardsTableRow(int pSuit, int pValue) : Suit(pSuit), Value(pValue)
    {
    }
};

struct PastRoundScoresTableRow
{
    int Round;
    int Player;
    int Score;
    PastRoundScoresTableRow(int pRound, int pPlayer, int pScore) : Round(pRound), Player(pPlayer), Score(pScore)
    {
    }
};

struct RoundScoresTableRow
{
    int Player;
    int Score;
    RoundScoresTableRow(int pPlayer, int pScore) : Player(pPlayer), Score(pScore)
    {
    }
};

struct OverallScoresTableRow
{
    int Player;
    int Score;
    OverallScoresTableRow(int pPlayer, int pScore) : Player(pPlayer), Score(pScore)
    {
    }
};

struct TeamsTableRow
{
    int Team;
    int Player;
};

struct TeamScoresTableRow
{
    int Team;
    int Score;
};

struct CurrentRoundInfoTableRow
{
    int Round;
    int BidPlayer;
    int BidAmount;
    int PartnerSuit;
    int PartnerValue;
    int TrumpSuit;
    int PointsMiddle;

    CurrentRoundInfoTableRow(int pRound, int pBidPlayer, int pBidAmount, int pPartnerSuit,
                             int pPartnerValue, int pTrumpSuit, int pPointsMiddle) : Round(pRound), BidPlayer(pBidPlayer),
                                                                                     BidAmount(pBidAmount), PartnerSuit(pPartnerSuit),
                                                                                     PartnerValue(pPartnerValue), TrumpSuit(pTrumpSuit),
                                                                                     PointsMiddle(pPointsMiddle)
    {
    }
};

struct HandInfoTableRow
{
    int StartingPlayerNum;
    int Player1CardSuit;
    int Player1CardValue;
    int Player2CardSuit;
    int Player2CardValue;
    int Player3CardSuit;
    int Player3CardValue;
    int Player4CardSuit;
    int Player4CardValue;
    int HandSuit;
    int HandPoints;

    HandInfoTableRow(int pStartingPlayerNum,
                     int pPlayer1CardSuit, int pPlayer1CardValue, int pPlayer2CardSuit, int pPlayer2CardValue,
                     int pPlayer3CardSuit, int pPlayer3CardValue, int pPlayer4CardSuit, int pPlayer4CardValue,
                     int pHandSuit, int pHandPoints) : StartingPlayerNum(pStartingPlayerNum),
                                                       Player1CardSuit(pPlayer1CardSuit), Player1CardValue(pPlayer1CardValue),
                                                       Player2CardSuit(pPlayer2CardSuit), Player2CardValue(pPlayer2CardValue),
                                                       Player3CardSuit(pPlayer3CardSuit), Player3CardValue(pPlayer3CardValue),
                                                       Player4CardSuit(pPlayer4CardSuit), Player4CardValue(pPlayer4CardValue),
                                                       HandSuit(pHandSuit), HandPoints(pHandPoints)
    {
    }
};

// prefer "bind" to INSERT
class GameDatabase
{
    void dropTable(const std::string &tableName);

    void createTablePlayerCards();
    void createTablePlayerBids();
    void createTableNestCards();
    void createTablePastRoundScores();
    void createTableRoundScores();
    void createTableOverallScores();
    void createTableTeams();
    void createTableTeamScores();
    void createTableCurrentRoundInfo();
    void createTableHandInfo();

    // write to db
    void populateTablePlayerCards(const std::array<Player, 4> &playerArr);
    void populateTablePlayerBids(const std::array<Player, 4> &playerArr);
    void populateTableNestCards(const std::vector<Card> &cardArr);
    void populateTablePastRoundScores(const std::map<int, std::map<int, int>> &pastRoundScores);
    void populateTableRoundScores(const std::map<int, int> &roundScores);
    void populateTableOverallScores(const std::map<int, int> &playerScores);
    void populateTableTeams(const std::array<Team, 2> &teams);
    void populateTableTeamScores(const std::map<int, int> &teamScores);
    void populateTableCurrentRoundInfo(int round, int bidPlayer, int bidAmount, const Card &partnerCard, int trump, int pointsMiddle);
    void populateTableHandInfo(int startingPlayerNum, const std::map<int, Card> &cardPlayed, int suit, int points);

    // read from db
    void loadTablePlayerCards(std::array<Player, 4> &playerArr);
    void loadTablePlayerBids(std::array<Player, 4> &playerArr);
    void loadTableNestCards(std::vector<Card> &cardArr);
    void loadTablePastRoundScores(std::map<int, std::map<int, int>> &pastRoundScores);
    void loadTableRoundScores(std::map<int, int> &playerScores);
    void loadTableOverallScores(std::map<int, int> &playerScores);
    void loadTableTeams(std::array<Team, 2> &teams, std::array<Player, 4> &playerArr); // we also modify playerArr here
    void loadTableTeamScores(std::map<int, int> &teamScores);
    void loadTableCurrentRoundInfo(int &round, int &bidPlayer, int &bidAmount, Card &partnerCard, int &trump, int &pointsMiddle);
    void loadTableHandInfo(int &startingPlayerNum, std::map<int, Card> &cardPlayed, int &suit, int &points);

public:
    GameDatabase();

    void DatabaseConnect(const QString &dbName);

    void SaveToDb(const QString &dbName, GameData &data, QProgressBar *progressBar);
    void DatabaseInit();                   // create tables
    void DatabasePopulate(GameData &data); // insert into tables

    void LoadFromDb(const QString &dbName, GameData &data);
    void DatabaseLoad(GameData &data); // select table rows
};

#endif