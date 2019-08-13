#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

using namespace std;

#include "gameController.h"
#include "gameDatabase.h"

// global declarations
extern GameController gc;

GameDatabase::GameDatabase()
{
}

void GameDatabase::DatabaseConnect(const QString &dbName)
{
    const QString DRIVER("QSQLITE");

    if (QSqlDatabase::isDriverAvailable(DRIVER))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName(dbName); // use :memory: to keep data in memory

        if (!db.open())
        {
            qWarning() << "GameDatabase::DatabaseConnect - ERROR: " << db.lastError().text();
        }
    }
    else
    {
        qWarning() << "GameDatabase::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
    }
}

void GameDatabase::SaveToDb(const QString &dbName, GameData &data, QProgressBar *progressBar)
{
    DatabaseConnect(dbName);
    progressBar->setValue(33);

    DatabaseInit();
    progressBar->setValue(66); 
    
    DatabasePopulate(data);
    progressBar->setValue(100);
}

void GameDatabase::DatabaseInit()
{
    dropTable("PlayerCards");
    dropTable("PlayerBids");
    dropTable("NestCards");
    dropTable("PastRoundScores");
    dropTable("RoundScores");
    dropTable("OverallScores");
    dropTable("Teams");
    dropTable("TeamScores");
    dropTable("CurrentRoundInfo");
    dropTable("HandInfo");

    createTablePlayerCards();
    createTablePlayerBids();
    createTableNestCards();
    createTablePastRoundScores();
    createTableRoundScores();
    createTableOverallScores();
    createTableTeams();
    createTableTeamScores();
    createTableCurrentRoundInfo();
    createTableHandInfo();
}

void GameDatabase::DatabasePopulate(GameData &data)
{
    populateTablePlayerCards(data.playerArr);

    populateTablePlayerBids(data.playerArr);

    populateTableNestCards(data.nest);

    populateTablePastRoundScores(data.scoreHistory);

    populateTableRoundScores(data.roundInfo.playerScores);

    populateTableOverallScores(data.overallInfo.playerScores);

    populateTableTeams(data.roundInfo.teams);

    populateTableTeamScores(data.roundInfo.teamScores);

    populateTableCurrentRoundInfo(data.overallInfo.roundNum,
                                  data.roundInfo.bidPlayer,
                                  data.roundInfo.bidAmount,
                                  data.roundInfo.partnerCard,
                                  data.roundInfo.trump,
                                  data.roundInfo.pointsMiddle);

    populateTableHandInfo(data.handInfo.winningCard,
                          data.handInfo.winningPlayerNum,
                          data.handInfo.startingPlayerNum,
                          data.handInfo.cardPlayed,
                          data.handInfo.suit,
                          data.handInfo.points);
}

void GameDatabase::LoadFromDb(const QString &dbName, GameData &data)
{
    DatabaseConnect(dbName);
    DatabaseLoad(data);
}

void GameDatabase::DatabaseLoad(GameData &data)
{
    loadTablePlayerCards(data.playerArr);

    loadTablePlayerBids(data.playerArr);

    loadTableNestCards(data.nest);

    loadTablePastRoundScores(data.scoreHistory);

    loadTableRoundScores(data.roundInfo.playerScores);

    loadTableOverallScores(data.overallInfo.playerScores);

    loadTableTeams(data.roundInfo.teams, data.playerArr);

    loadTableTeamScores(data.roundInfo.teamScores);

    loadTableCurrentRoundInfo(data.overallInfo.roundNum,
                              data.roundInfo.bidPlayer,
                              data.roundInfo.bidAmount,
                              data.roundInfo.partnerCard,
                              data.roundInfo.trump,
                              data.roundInfo.pointsMiddle);

    loadTableHandInfo(data.handInfo.winningCard,
                      data.handInfo.winningPlayerNum,
                      data.handInfo.startingPlayerNum,
                      data.handInfo.cardPlayed,
                      data.handInfo.suit,
                      data.handInfo.points);
}

void GameDatabase::createTablePlayerCards()
{
    QSqlQuery query("CREATE TABLE PlayerCards (id INTEGER PRIMARY KEY, Player INTEGER, Suit INTEGER, Value INTEGER)");

    if (!query.isActive())
    {
        qWarning() << "GameDatabase::createTablePlayerCards - ERROR: " << query.lastError().text();
    }
}

void GameDatabase::createTablePlayerBids()
{
    QSqlQuery query("CREATE TABLE PlayerBids (id INTEGER PRIMARY KEY, Player INTEGER, Bid INTEGER)");

    if (!query.isActive())
    {
        qWarning() << "GameDatabase::createTablePlayerBids - ERROR: " << query.lastError().text();
    }
}

void GameDatabase::createTableNestCards()
{
    QSqlQuery query("CREATE TABLE NestCards (id INTEGER PRIMARY KEY, Suit INTEGER, Value INTEGER)");

    if (!query.isActive())
    {
        qWarning() << "GameDatabase::createTableNestCards - ERROR: " << query.lastError().text();
    }
}

void GameDatabase::createTablePastRoundScores()
{
    QSqlQuery query("CREATE TABLE PastRoundScores (id INTEGER PRIMARY KEY, Round INTEGER, Player INTEGER, Score INTEGER)");

    if (!query.isActive())
    {
        qWarning() << "GameDatabase::createTablePastRoundScores - ERROR: " << query.lastError().text();
    }
}

void GameDatabase::createTableRoundScores()
{
    QSqlQuery query("CREATE TABLE RoundScores (id INTEGER PRIMARY KEY, Player INTEGER, Score INTEGER)");

    if (!query.isActive())
    {
        qWarning() << "GameDatabase::createTableRoundScores - ERROR: " << query.lastError().text();
    }
}

void GameDatabase::createTableOverallScores()
{
    QSqlQuery query("CREATE TABLE OverallScores (id INTEGER PRIMARY KEY, Player INTEGER, Score INTEGER)");

    if (!query.isActive())
    {
        qWarning() << "GameDatabase::createTableOverallScores - ERROR: " << query.lastError().text();
    }
}

void GameDatabase::createTableTeams()
{
    QSqlQuery query("CREATE TABLE Teams (id INTEGER PRIMARY KEY, Team INTEGER, Player INTEGER)");

    if (!query.isActive())
    {
        qWarning() << "GameDatabase::createTableTeams - ERROR: " << query.lastError().text();
    }
}

void GameDatabase::createTableTeamScores()
{
    QSqlQuery query("CREATE TABLE TeamScores (id INTEGER PRIMARY KEY, Team INTEGER, Score INTEGER)");

    if (!query.isActive())
    {
        qWarning() << "GameDatabase::createTableTeamScores - ERROR: " << query.lastError().text();
    }
}

void GameDatabase::createTableCurrentRoundInfo()
{
    QSqlQuery query("CREATE TABLE CurrentRoundInfo (id INTEGER PRIMARY KEY, Round INTEGER"
                    ", BidPlayer INTEGER, BidAmount INTEGER, PartnerSuit INTEGER, PartnerValue INTEGER, "
                    "TrumpSuit INTEGER, PointsMiddle INTEGER)");

    if (!query.isActive())
    {
        qWarning() << "GameDatabase::createTableCurrentRoundInfo - ERROR: " << query.lastError().text();
    }
}

void GameDatabase::createTableHandInfo()
{
    QSqlQuery query("CREATE TABLE HandInfo (id INTEGER PRIMARY KEY, WinningCardSuit INTEGER, WinningCardValue INTEGER, "
                    "WinningPlayerNum INTEGER, StartingPlayerNum INTEGER, Player1CardSuit INTEGER, Player1CardValue INTEGER, "
                    "Player2CardSuit INTEGER, Player2CardValue INTEGER, Player3CardSuit INTEGER, Player3CardValue INTEGER, "
                    "Player4CardSuit INTEGER, Player4CardValue INTEGER, HandSuit INTEGER, HandPoints INTEGER)");

    if (!query.isActive())
    {
        qWarning() << "GameDatabase::createTableHandInfo - ERROR: " << query.lastError().text();
    }
}

void GameDatabase::populateTablePlayerCards(const array<Player, 4> &playerArr)
{
    vector<PlayerCardsTableRow> tableRows;

    for (auto &player : playerArr)
    {
        for (auto &card : player.cardArr)
        {
            tableRows.push_back({player.playerNum, card.suit, card.value});
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO PlayerCards(Player, Suit, Value) VALUES(?,?,?)");
    QSqlDatabase::database().transaction();

    for (auto &row : tableRows)
    {
        query.bindValue(0, row.Player);
        query.bindValue(1, row.Suit);
        query.bindValue(2, row.Value);

        if (!query.exec())
        {
            qWarning() << "GameDatabase::populateTablePlayerCards - ERROR: " << query.lastError().text();
        }
    }

    QSqlDatabase::database().commit();
}

void GameDatabase::populateTablePlayerBids(const array<Player, 4> &playerArr)
{
    vector<PlayerBidsTableRow> tableRows;

    for (auto &player : playerArr)
    {
        tableRows.push_back({player.playerNum, player.bid});
    }

    QSqlQuery query;
    query.prepare("INSERT INTO PlayerBids(Player, Bid) VALUES(?,?)");
    QSqlDatabase::database().transaction();

    for (auto &row : tableRows)
    {
        query.bindValue(0, row.Player);
        query.bindValue(1, row.Bid);

        if (!query.exec())
        {
            qWarning() << "GameDatabase::populateTablePlayerBids - ERROR: " << query.lastError().text();
        }
    }

    QSqlDatabase::database().commit();
}

void GameDatabase::populateTableNestCards(const vector<Card> &cardArr)
{
    vector<NestCardsTableRow> tableRows;

    for (auto &card : cardArr)
    {
        tableRows.push_back({card.suit, card.value});
    }

    QSqlQuery query;
    query.prepare("INSERT INTO NestCards(Suit, Value) VALUES(?,?)");
    QSqlDatabase::database().transaction();

    for (auto &row : tableRows)
    {
        query.bindValue(0, row.Suit);
        query.bindValue(1, row.Value);

        if (!query.exec())
        {
            qWarning() << "GameDatabase::populateTableNestCards - ERROR: " << query.lastError().text();
        }
    }

    QSqlDatabase::database().commit();
}

void GameDatabase::populateTablePastRoundScores(map<int, map<int, int>> &pastRoundScores)
{
    vector<PastRoundScoresTableRow> tableRows;

    for(auto &round : pastRoundScores)
    {
        for(auto &playerNo : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
        {
            tableRows.push_back({round.first, playerNo, round.second[playerNo]});
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO PastRoundScores(Round, Player, Score) VALUES(?,?,?)");
    QSqlDatabase::database().transaction();

    for(auto &row : tableRows)
    {
        query.bindValue(0, row.Round);
        query.bindValue(1, row.Player);
        query.bindValue(2, row.Score);

        if (!query.exec())
        {
            qWarning() << "GameDatabase::populateTablePastRoundScores - ERROR: " << query.lastError().text();
        }
    }

    QSqlDatabase::database().commit();
}

void GameDatabase::populateTableRoundScores(map<int, int> &roundScores)
{
    vector<RoundScoresTableRow> tableRows;

    for (auto &playerNo : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        tableRows.push_back({playerNo, roundScores[playerNo]});
    }

    QSqlQuery query;
    query.prepare("INSERT INTO RoundScores(Player, Score) VALUES(?,?)");
    QSqlDatabase::database().transaction();

    for (auto &row : tableRows)
    {
        query.bindValue(0, row.Player);
        query.bindValue(1, row.Score);

        if (!query.exec())
        {
            qWarning() << "GameDatabase::populateTableRoundScores - ERROR: " << query.lastError().text();
        }
    }

    QSqlDatabase::database().commit();
}

void GameDatabase::populateTableOverallScores(map<int, int> &playerScores)
{
    vector<OverallScoresTableRow> tableRows;

    for (auto player : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        tableRows.push_back({player, playerScores[player]});
    }

    QSqlQuery query;
    query.prepare("INSERT INTO OverallScores(Player, Score) VALUES(?,?)");
    QSqlDatabase::database().transaction();

    for (auto &row : tableRows)
    {
        string sql = "INSERT INTO OverallScores(Player, Score) VALUES(" +
                     to_string(row.Player) + "," +
                     to_string(row.Score) + ")";

        if (!query.exec(QString::fromStdString(sql)))
        {
            qWarning() << "GameDatabase::populateTableOverallScores - ERROR: " << query.lastError().text();
        }
    }

    QSqlDatabase::database().commit();
}

void GameDatabase::populateTableTeams(const array<Team, 2> &teams)
{
    vector<TeamsTableRow> tableRows;

    for(auto teamNum : vector<int>{TEAM_1, TEAM_2})
    {
        for(auto playerNum : teams[teamNum])
        {
            tableRows.push_back({teamNum, playerNum});
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Teams(Team, Player) VALUES(?,?)");
    QSqlDatabase::database().transaction();

    for (auto &row : tableRows)
    {
        query.bindValue(0, row.Team);
        query.bindValue(1, row.Player);

        if (!query.exec())
        {
            qWarning() << "GameDatabase::populateTableTeams - ERROR: " << query.lastError().text();
        }
    }

    QSqlDatabase::database().commit();
}

void GameDatabase::populateTableTeamScores(map<int, int> &teamScores)
{
    vector<TeamScoresTableRow> tableRows;

    for (auto teamNum : vector<int>{TEAM_1, TEAM_2})
    {
        tableRows.push_back({teamNum, teamScores[teamNum]});
    }

    QSqlQuery query;
    query.prepare("INSERT INTO TeamScores(Team, Score) VALUES(?,?)");
    QSqlDatabase::database().transaction();

    for (auto &row : tableRows)
    {
        query.bindValue(0, row.Team);
        query.bindValue(1, row.Score);

        if (!query.exec())
        {
            qWarning() << "GameDatabase::populateTableTeamScores - ERROR: " << query.lastError().text();
        }
    }

    QSqlDatabase::database().commit();
}

void GameDatabase::populateTableCurrentRoundInfo(int round, int bidPlayer, int bidAmount, const Card &partnerCard, int trump, int pointsMiddle)
{
    vector<CurrentRoundInfoTableRow> tableRows;

    tableRows.push_back({round, bidPlayer, bidAmount, partnerCard.suit, partnerCard.value, trump, pointsMiddle});

    QSqlQuery query;
    query.prepare("INSERT INTO CurrentRoundInfo(Round, BidPlayer, BidAmount, PartnerSuit, PartnerValue, TrumpSuit, PointsMiddle) VALUES(?,?,?,?,?,?,?)");
    QSqlDatabase::database().transaction();

    for (auto &row : tableRows)
    {
        query.bindValue(0, row.Round);
        query.bindValue(1, row.BidPlayer);
        query.bindValue(2, row.BidAmount);
        query.bindValue(3, row.PartnerSuit);
        query.bindValue(4, row.PartnerValue);
        query.bindValue(5, row.TrumpSuit);
        query.bindValue(6, row.PointsMiddle);

        if (!query.exec())
        {
            qWarning() << "GameDatabase::populateTableCurrentRoundInfo - ERROR: " << query.lastError().text();
        }
    }

    QSqlDatabase::database().commit();
}

void GameDatabase::populateTableHandInfo(const Card &winningCard, int winningPlayerNum, int startingPlayerNum, map<int, Card> &cardPlayed, int suit, int points)
{
    vector<HandInfoTableRow> tableRows;

    tableRows.push_back({winningCard.suit, winningCard.value, winningPlayerNum, startingPlayerNum,
                         cardPlayed[PLAYER_1].suit, cardPlayed[PLAYER_1].value, cardPlayed[PLAYER_2].suit, cardPlayed[PLAYER_2].value,
                         cardPlayed[PLAYER_3].suit, cardPlayed[PLAYER_3].value, cardPlayed[PLAYER_4].suit, cardPlayed[PLAYER_4].value,
                         suit, points});

    QSqlQuery query;
    query.prepare("INSERT INTO HandInfo(WinningCardSuit, WinningCardValue, WinningPlayerNum, StartingPlayerNum, "
                  "Player1CardSuit, Player1CardValue, Player2CardSuit, Player2CardValue, "
                  "Player3CardSuit, Player3CardValue, Player4CardSuit, Player4CardValue, "
                  "HandSuit, HandPoints) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    QSqlDatabase::database().transaction();

    for (auto &row : tableRows)
    {
        query.bindValue(0, row.WinningCardSuit);
        query.bindValue(1, row.WinningCardValue);
        query.bindValue(2, row.WinningPlayerNum);
        query.bindValue(3, row.StartingPlayerNum);
        query.bindValue(4, row.Player1CardSuit);
        query.bindValue(5, row.Player1CardValue);
        query.bindValue(6, row.Player2CardSuit);
        query.bindValue(7, row.Player2CardValue);
        query.bindValue(8, row.Player3CardSuit);
        query.bindValue(9, row.Player3CardValue);
        query.bindValue(10, row.Player4CardSuit);
        query.bindValue(11, row.Player4CardValue);
        query.bindValue(12, row.HandSuit);
        query.bindValue(13, row.HandPoints);

        if (!query.exec())
        {
            qWarning() << "GameDatabase::populateTableHandInfo - ERROR: " << query.lastError().text();
        }
    }

    QSqlDatabase::database().commit();
}

void GameDatabase::loadTablePlayerCards(array<Player, 4> &playerArr)
{
    for (auto &player : playerArr)
    {
        player.cardArr.clear();
    }

    QSqlQuery query;

    if (!query.exec("SELECT Player, Suit, Value FROM PlayerCards"))
    {
        qWarning() << "GameDatabase::loadTablePlayerCards - ERROR: " << query.lastError().text();
    }

    while (query.next())
    {
        int playerNo = query.value(0).toInt();
        int suit = query.value(1).toInt();
        int value = query.value(2).toInt();
        playerArr[playerNo].cardArr.push_back({suit, value});
    }
}

void GameDatabase::loadTablePlayerBids(array<Player, 4> &playerArr)
{
    for (auto &player : playerArr)
    {
        player.bid = 0;
    }

    QSqlQuery query;

    if (!query.exec("SELECT Player, Bid FROM PlayerBids"))
    {
        qWarning() << "GameDatabase::loadTablePlayerBids - ERROR: " << query.lastError().text();
    }

    while (query.next())
    {
        int playerNo = query.value(0).toInt();
        int bid = query.value(1).toInt();
        playerArr[playerNo].bid = bid;
    }
}

void GameDatabase::loadTableNestCards(vector<Card> &cardArr)
{
    cardArr.clear();

    QSqlQuery query;

    if (!query.exec("SELECT Suit, Value FROM NestCards"))
    {
        qWarning() << "GameDatabase::loadTableNestCards - ERROR: " << query.lastError().text();
    }

    while (query.next())
    {
        int suit = query.value(0).toInt();
        int value = query.value(1).toInt();
        cardArr.push_back({suit, value});
    }
}

void GameDatabase::loadTablePastRoundScores(map<int, map<int, int>> &pastRoundScores)
{
    pastRoundScores.clear();

    QSqlQuery query;

    if (!query.exec("SELECT Round, Player, Score FROM PastRoundScores"))
    {
        qWarning() << "GameDatabase::loadTablePastRoundScores - ERROR: " << query.lastError().text();
    }

    while (query.next())
    {
        int roundNo = query.value(0).toInt();
        int playerNo = query.value(1).toInt();
        int score = query.value(2).toInt();
        pastRoundScores[roundNo][playerNo] = score;
    }
}

void GameDatabase::loadTableRoundScores(map<int, int> &playerScores)
{
    playerScores.clear();

    QSqlQuery query;

    if (!query.exec("SELECT Player, Score FROM RoundScores"))
    {
        qWarning() << "GameDatabase::loadTableOverallScores - ERROR: " << query.lastError().text();
    }

    while (query.next())
    {
        int playerNo = query.value(0).toInt();
        int score = query.value(1).toInt();
        playerScores[playerNo] = score;
    }
}

void GameDatabase::loadTableOverallScores(map<int, int> &playerScores)
{
    playerScores.clear();

    QSqlQuery query;

    if (!query.exec("SELECT Player, Score FROM OverallScores"))
    {
        qWarning() << "GameDatabase::loadTableOverallScores - ERROR: " << query.lastError().text();
    }

    while (query.next())
    {
        int playerNo = query.value(0).toInt();
        int score = query.value(1).toInt();
        playerScores[playerNo] = score;
    }
}

void GameDatabase::loadTableTeams(array<Team, 2> &teams, array<Player, 4> &playerArr)
{
    for(auto &team : teams)
    {
        team.clear();
    }

    QSqlQuery query;

    if (!query.exec("SELECT Team, Player FROM Teams"))
    {
        qWarning() << "GameDatabase::loadTableTeams - ERROR: " << query.lastError().text();
    }

    while (query.next())
    {
        int teamNo = query.value(0).toInt();
        int player = query.value(1).toInt();

        teams[teamNo].insert(player);
    }

    for(auto teamNum : vector<int>{TEAM_1, TEAM_2})
    {
        for(auto playerNum : teams[teamNum])
        {
            playerArr[playerNum].teamNum = teamNum;
        }
    }
}

void GameDatabase::loadTableTeamScores(map<int, int> &teamScores)
{
    teamScores.clear();

    QSqlQuery query;

    if (!query.exec("SELECT Team, Score FROM TeamScores"))
    {
        qWarning() << "GameDatabase::loadTableTeamScores - ERROR: " << query.lastError().text();
    }

    while (query.next())
    {
        int teamNo = query.value(0).toInt();
        int score = query.value(1).toInt();

        teamScores[teamNo] = score;
    }
}

void GameDatabase::loadTableCurrentRoundInfo(int &round, int &bidPlayer, int &bidAmount, Card &partnerCard, int &trump, int &pointsMiddle)
{
    round = 0;
    bidPlayer = 0;
    bidAmount = 0;
    partnerCard = Card(SUIT_UNDEFINED, VALUE_UNDEFINED);
    trump = SUIT_UNDEFINED;
    pointsMiddle = 0;

    QSqlQuery query;

    if (!query.exec("SELECT Round, BidPlayer, BidAmount, PartnerSuit, PartnerValue, TrumpSuit, PointsMiddle FROM CurrentRoundInfo"))
    {
        qWarning() << "GameDatabase::loadTableCurrentRoundInfo - ERROR: " << query.lastError().text();
    }

    while (query.next())
    {
        round = query.value(0).toInt();
        bidPlayer = query.value(1).toInt();
        bidAmount = query.value(2).toInt();
        partnerCard.suit = query.value(3).toInt();
        partnerCard.value = query.value(4).toInt();
        trump = query.value(5).toInt();
        pointsMiddle = query.value(6).toInt();
    }
}

void GameDatabase::loadTableHandInfo(Card &winningCard, int &winningPlayerNum, int &startingPlayerNum, map<int, Card> &cardPlayed, int &suit, int &points)
{
    winningCard = Card(SUIT_UNDEFINED, VALUE_UNDEFINED);
    winningPlayerNum = PLAYER_UNDEFINED;
    startingPlayerNum = PLAYER_UNDEFINED;
    cardPlayed.clear();
    suit = SUIT_UNDEFINED;
    points = 0;

    QSqlQuery query;

    if (!query.exec("SELECT WinningCardSuit, WinningCardValue, WinningPlayerNum, StartingPlayerNum, "
                    "Player1CardSuit, Player1CardValue, Player2CardSuit, Player2CardValue, "
                    "Player3CardSuit, Player3CardValue, Player4CardSuit, Player4CardValue, "
                    "HandSuit, HandPoints FROM HandInfo"))
    {
        qWarning() << "gameDatabase::loadTableHandInfo - ERROR: " << query.lastError().text();
    }

    while (query.next())
    {
        winningCard = Card(query.value(0).toInt(), query.value(1).toInt());
        winningPlayerNum = query.value(2).toInt();
        startingPlayerNum = query.value(3).toInt();
        cardPlayed[PLAYER_1] = Card(query.value(4).toInt(), query.value(5).toInt());
        cardPlayed[PLAYER_2] = Card(query.value(6).toInt(), query.value(7).toInt());
        cardPlayed[PLAYER_3] = Card(query.value(8).toInt(), query.value(9).toInt());
        cardPlayed[PLAYER_4] = Card(query.value(10).toInt(), query.value(11).toInt());
        suit = query.value(12).toInt();
        points = query.value(13).toInt();
    }
}

void GameDatabase::dropTable(const string &tableName)
{
    string sql = "DROP TABLE IF EXISTS " + tableName;

    QSqlQuery dropQuery(QString::fromStdString(sql));

    if (!dropQuery.exec())
    {
        qWarning() << "GameDatabase::dropTable - ERROR: " << dropQuery.lastError().text();
    }
}