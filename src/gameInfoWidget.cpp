#include <QToolTip>

#include "card.h"
#include "gameInfoWidget.h"
#include "player.h"
#include "settings.h"
#include "utils.h"

GameInfoWidget::GameInfoWidget(QMainWindow *pMainWindow, QWidget *parent) : mainWindow(pMainWindow),
                                                                            QDialogWithClickableCardArray(false, parent)
{
    clearDataVariables();

    partnerCards = new ClickableCardArray(DRAW_POSITION_GAME_INFO_WIDGET, SIZE_TINY, this);

    auto setupCategoryLabel = [this](ScaledQLabel *categoryLabel, QString text, QSize size, QPoint pos) {
        categoryLabel->setParent(this);
        categoryLabel->setText(text);
        categoryLabel->setFont(QFont("Times", 12, QFont::Weight::Bold));
        categoryLabel->resize(size.width(), size.height());
        categoryLabel->move(pos);
    };

    auto setupLabel = [this](ScaledQLabel *label, QString text, QSize size, QPoint pos) {
        label->setParent(this);
        label->setText(text);
        label->setFont(QFont("Times", 10, QFont::Weight::Normal));
        label->resize(size.width(), size.height());
        label->move(pos);
    };

    bidCategoryLabel = new ScaledQLabel;
    setupCategoryLabel(bidCategoryLabel, "Bid", {100, 20}, {25, 10});

    bidPlayerLabel = new ScaledQLabel;
    setupLabel(bidPlayerLabel, "???", {100, 20}, {35, 35});

    bidAmountLabel = new ScaledQLabel;
    setupLabel(bidAmountLabel, "???", {100, 20}, {35, 55});

    partnerCardCategoryLabel = new ScaledQLabel;
    setupCategoryLabel(partnerCardCategoryLabel, "Partner", {100, 20}, {135, 10});

    trumpCategoryLabel = new ScaledQLabel;
    setupCategoryLabel(trumpCategoryLabel, "Trump", {100, 20}, {270, 10});

    trumpLabel = new ScaledQLabel;
    setupLabel(trumpLabel, "???", {50, 20}, {280, 35});
    trumpLabel->setAlignment(Qt::AlignHCenter);

    pointsMiddleCategoryLabel = new ScaledQLabel;
    setupCategoryLabel(pointsMiddleCategoryLabel, "Points Middle", {125, 20}, {380, 10});

    pointsMiddleLabel = new ScaledQLabel;
    setupLabel(pointsMiddleLabel, "???", {125, 20}, {390, 35});

    teamsCategoryLabel = new ScaledQLabel;
    setupCategoryLabel(teamsCategoryLabel, "Teams", {100, 20}, {530, 10});

    team1Label = new ScaledQLabel;
    setupLabel(team1Label, "???", {150, 20}, {540, 35});

    team2Label = new ScaledQLabel;
    setupLabel(team2Label, "???", {150, 20}, {540, 55});

    pointsWonTeamCategoryLabel = new ScaledQLabel;
    setupCategoryLabel(pointsWonTeamCategoryLabel, "Points (by Team)", {150, 20}, {690, 10});

    pointsWonTeamLabel1 = new ScaledQLabel;
    setupLabel(pointsWonTeamLabel1, "???", {150, 20}, {700, 35});

    pointsWonTeamLabel2 = new ScaledQLabel;
    setupLabel(pointsWonTeamLabel2, "???", {150, 20}, {700, 55});

    pointsWonPlayerCategoryLabel = new ScaledQLabel;
    setupCategoryLabel(pointsWonPlayerCategoryLabel, "Points (by Player)", {160, 20}, {890, 10});

    pointsWonPlayerLabel1 = new ScaledQLabel;
    setupLabel(pointsWonPlayerLabel1, "???", {100, 20}, {900, 35});

    pointsWonPlayerLabel2 = new ScaledQLabel;
    setupLabel(pointsWonPlayerLabel2, "???", {100, 20}, {900, 55});

    pointsWonPlayerLabel3 = new ScaledQLabel;
    setupLabel(pointsWonPlayerLabel3, "???", {100, 20}, {900, 75});

    pointsWonPlayerLabel4 = new ScaledQLabel;
    setupLabel(pointsWonPlayerLabel4, "???", {100, 20}, {900, 95});

    overallScoresCategoryLabel = new ScaledQLabel;
    setupCategoryLabel(overallScoresCategoryLabel, "Scores", {100, 20}, {1080, 10});

    player1OverallScoreLabel = new ScaledQLabel;
    setupLabel(player1OverallScoreLabel, "???", {100, 20}, {1090, 35});

    player2OverallScoreLabel = new ScaledQLabel;
    setupLabel(player2OverallScoreLabel, "???", {100, 20}, {1090, 55});

    player3OverallScoreLabel = new ScaledQLabel;
    setupLabel(player3OverallScoreLabel, "???", {100, 20}, {1090, 75});

    player4OverallScoreLabel = new ScaledQLabel;
    setupLabel(player4OverallScoreLabel, "???", {100, 20}, {1090, 95});

    setStyleSheet("background-color: white");
    resize(GAME_INFO_WIDGET_SIZE);
}

void GameInfoWidget::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto label : vector<ScaledQLabel *>{bidCategoryLabel, bidPlayerLabel, bidAmountLabel, partnerCardCategoryLabel,
                                             trumpCategoryLabel, trumpLabel, pointsMiddleCategoryLabel, pointsMiddleLabel,
                                             teamsCategoryLabel, team1Label, team2Label, pointsWonPlayerCategoryLabel,
                                             pointsWonPlayerLabel1, pointsWonPlayerLabel2, pointsWonPlayerLabel3,
                                             pointsWonPlayerLabel4, pointsWonTeamCategoryLabel, pointsWonTeamLabel1,
                                             pointsWonTeamLabel2, overallScoresCategoryLabel, player1OverallScoreLabel,
                                             player2OverallScoreLabel, player3OverallScoreLabel, player4OverallScoreLabel})
        label->rescale();

    for (auto clickableCardArray : vector<ClickableCardArray *>{partnerCards})
        clickableCardArray->rescale();
}

void GameInfoWidget::resetRoundInfoToDefaults()
{
    updateBid(PLAYER_UNDEFINED, 0);
    updatePartner(Card());
    updateTrump(SUIT_UNDEFINED);
    updatePointsMiddle(0, false);
    updateTeam1({});
    updateTeam2({});
    updatePlayerPoints(map<int, int>());
    updateTeamPoints(map<int, int>());
}

void GameInfoWidget::resetOverallInfoToDefaults()
{
    updateOverallScores(map<int, int>());
}

void GameInfoWidget::resetInfoToDefaults()
{
    resetRoundInfoToDefaults();
    resetOverallInfoToDefaults();
}

void GameInfoWidget::onCardClicked(ClickableCard *clickableCard)
{
    // do nothing
}

void GameInfoWidget::onCardHoverEnter(ClickableCard *clickableCard)
{
    if (Settings::Appearance::readShowPartnerToolTip())
    {
        auto toolTipText = [this](int playerNum) -> QString {
            switch (playerNum)
            {
            case PLAYER_1:
            case PLAYER_2:
            case PLAYER_3:
            case PLAYER_4:
                return QString::fromStdString(playerNames[playerNum]);
            default:
                return "???";
            }
        }(partnerPlayerNum);

        QToolTip::showText(mainWindow->pos() + clickableCard->pos(), toolTipText, clickableCard);
    }
}

void GameInfoWidget::onCardHoverLeave(ClickableCard *clickableCard)
{
    // do nothing
}

void GameInfoWidget::updatePlayerNames(map<int, string> pPlayerNames)
{
    playerNames = pPlayerNames;

    updatePlayerPoints(playerScores);
    updateOverallScores(overallPlayerScores);
    updateTeam1(teams.first);
    updateTeam2(teams.second);
    updateTeamPoints(teamScores);
}

void GameInfoWidget::updateBid(int pBidPlayerNum, int pBidAmount)
{
    bidPlayerNum = pBidPlayerNum;
    bidAmount = pBidAmount;

    if (bidPlayerNum == PLAYER_UNDEFINED) // bid round hasn't started yet
    {
        bidPlayerLabel->setText("???");
        bidAmountLabel->setText("???");
    }
    else
    {
        bidPlayerLabel->setText(QString::fromStdString(playerNames[bidPlayerNum]));
        bidAmountLabel->setText(QString::number(bidAmount));
    }
}

void GameInfoWidget::updatePartner(Card pPartnerCard, int pPartnerPlayerNum)
{
    partnerCard = pPartnerCard;
    partnerPlayerNum = pPartnerPlayerNum;

    partnerCards->showCards({partnerCard});
}

void GameInfoWidget::updateTrump(int trumpSuit)
{
    switch (trumpSuit)
    {
    case SUIT_BLACK:
        trumpLabel->setStyleSheet("background-color: black; color: white");
        trumpLabel->setText("Black");
        break;
    case SUIT_GREEN:
        trumpLabel->setStyleSheet("background-color: green");
        trumpLabel->setText("Green");
        break;
    case SUIT_RED:
        trumpLabel->setStyleSheet("background-color: red");
        trumpLabel->setText("Red");
        break;
    case SUIT_YELLOW:
        trumpLabel->setStyleSheet("background-color: yellow");
        trumpLabel->setText("Yellow");
        break;
    default:
        trumpLabel->setStyleSheet("");
        trumpLabel->setText("???");
        break;
    }
}

void GameInfoWidget::updatePointsMiddle(int pPointsMiddle, bool pPointsMiddleKnown)
{
    pointsMiddle = pPointsMiddle;
    pointsMiddleKnown = pPointsMiddleKnown;

    if (!pointsMiddleKnown)
    {
        pointsMiddleLabel->setText("???");
    }
    else
    {
        pointsMiddleLabel->setText(QString::number(pointsMiddle));
    }
}

void GameInfoWidget::updateTeam1(Team team1)
{
    teams.first = team1;

    if (teams.first.empty())
    {
        team1Label->setText("???");
    }
    else
    {
        team1Label->setText(QString::fromStdString(getTeamName(teams.first)));
    }
}

void GameInfoWidget::updateTeam2(Team team2)
{
    teams.second = team2;

    if (teams.second.empty())
    {
        team2Label->setText("???");
    }
    else
    {
        team2Label->setText(QString::fromStdString(getTeamName(teams.second)));
    }
}

void GameInfoWidget::updateOverallScores(map<int, int> pOverallPlayerScores)
{
    overallPlayerScores = pOverallPlayerScores;

    auto setupLabel = [this](ScaledQLabel *label, int score, int playerNum) {
        string text = playerNames[playerNum] + ": " + to_string(score);
        label->setText(QString::fromStdString(text));
    };

    setupLabel(player1OverallScoreLabel, overallPlayerScores[PLAYER_1], PLAYER_1);
    setupLabel(player2OverallScoreLabel, overallPlayerScores[PLAYER_2], PLAYER_2);
    setupLabel(player3OverallScoreLabel, overallPlayerScores[PLAYER_3], PLAYER_3);
    setupLabel(player4OverallScoreLabel, overallPlayerScores[PLAYER_4], PLAYER_4);
}

void GameInfoWidget::updatePlayerPoints(map<int, int> pPlayerScores)
{
    playerScores = pPlayerScores;

    auto setupPointsPlayerLabel = [this](ScaledQLabel *label, int score, int playerNum) {
        string text = playerNames[playerNum] + ": " + to_string(score);
        label->setText(QString::fromStdString(text));
    };

    if (playerScores.empty())
    {
        setupPointsPlayerLabel(pointsWonPlayerLabel1, 0, PLAYER_1);
        setupPointsPlayerLabel(pointsWonPlayerLabel2, 0, PLAYER_2);
        setupPointsPlayerLabel(pointsWonPlayerLabel3, 0, PLAYER_3);
        setupPointsPlayerLabel(pointsWonPlayerLabel4, 0, PLAYER_4);
    }
    else
    {
        vector<pair<int, int>> sortedPlayerScores;

        for (auto It = playerScores.begin(); It != playerScores.end(); It++)
        {
            sortedPlayerScores.push_back(make_pair(It->first, It->second));
        }

        sort(sortedPlayerScores.begin(), sortedPlayerScores.end(), ScoreCompare());

        setupPointsPlayerLabel(pointsWonPlayerLabel1, sortedPlayerScores[0].second, sortedPlayerScores[0].first);
        setupPointsPlayerLabel(pointsWonPlayerLabel2, sortedPlayerScores[1].second, sortedPlayerScores[1].first);
        setupPointsPlayerLabel(pointsWonPlayerLabel3, sortedPlayerScores[2].second, sortedPlayerScores[2].first);
        setupPointsPlayerLabel(pointsWonPlayerLabel4, sortedPlayerScores[3].second, sortedPlayerScores[3].first);
    }
}

void GameInfoWidget::updateTeamPoints(map<int, int> pTeamScores)
{
    teamScores = pTeamScores;

    auto setupPointsTeamLabel = [this](ScaledQLabel *label, int score, Team team) {
        if (team.empty()) // teams not known yet
        {
            label->setText("???");
        }
        else
        {
            string text = getTeamName(team) + ": " + to_string(score);
            label->setText(QString::fromStdString(text));
        }
    };

    if (teamScores.empty() || teams.first.empty() || teams.second.empty())
    {
        setupPointsTeamLabel(pointsWonTeamLabel1, 0, Team());
        setupPointsTeamLabel(pointsWonTeamLabel2, 0, Team());
    }
    else
    {
        auto getTeam = [this](int teamNum) {
            switch (teamNum)
            {
            case TEAM_1:
                return teams.first;
            case TEAM_2:
                return teams.second;
            default:
                return Team();
            }
        };

        vector<pair<int, int>> sortedTeamScores;

        for (auto It = teamScores.begin(); It != teamScores.end(); It++)
        {
            sortedTeamScores.push_back(make_pair(It->first, It->second));
        }

        sort(sortedTeamScores.begin(), sortedTeamScores.end(), ScoreCompare());

        setupPointsTeamLabel(pointsWonTeamLabel1, sortedTeamScores[0].second, getTeam(sortedTeamScores[0].first));
        setupPointsTeamLabel(pointsWonTeamLabel2, sortedTeamScores[1].second, getTeam(sortedTeamScores[1].first));
    }
}

string GameInfoWidget::getTeamName(Team team)
{
    if (team.empty())
    {
        return "";
    }

    string teamName = "???";

    for (auto playerNum : team)
    {
        if (teamName == "???")
        {
            teamName = playerNames[playerNum];
        }
        else
        {
            teamName = teamName + " + " + playerNames[playerNum];
        }
    }

    return teamName;
}

void GameInfoWidget::clearDataVariables()
{
    playerNames.clear();
    bidPlayerNum = PLAYER_UNDEFINED;
    bidAmount = 0;
    partnerCard = Card(SUIT_UNDEFINED, VALUE_UNDEFINED);
    partnerPlayerNum = 0;
    trumpSuit = 0;
    pointsMiddle = 0;
    pointsMiddleKnown = false;
    teams.first.clear();
    teams.second.clear();
    playerScores.clear();
    teamScores.clear();
    overallPlayerScores.clear();
}