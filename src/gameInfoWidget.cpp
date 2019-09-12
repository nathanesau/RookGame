#include "card.h"
#include "gameData.h"
#include "gameInfoWidget.h"
#include "player.h"
#include "settings.h"
#include "utils.h"

GameInfoWidgetData::GameInfoWidgetData()
{
    bidPlayerNum = PLAYER_UNDEFINED;
    bidAmount = 0;
    partnerPlayerNum = PLAYER_UNDEFINED;
    trumpSuit = 0;
    pointsMiddle = 0;
    pointsMiddleKnown = false;
}

GameInfoWidget::GameInfoWidget(QWidget *parent) : ScaledQDialog(false, parent)
{
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

    for (auto label : std::vector<ScaledQLabel *>{bidCategoryLabel, bidPlayerLabel, bidAmountLabel, partnerCardCategoryLabel,
                                                  trumpCategoryLabel, trumpLabel, pointsMiddleCategoryLabel, pointsMiddleLabel,
                                                  teamsCategoryLabel, team1Label, team2Label, pointsWonPlayerCategoryLabel,
                                                  pointsWonPlayerLabel1, pointsWonPlayerLabel2, pointsWonPlayerLabel3,
                                                  pointsWonPlayerLabel4, pointsWonTeamCategoryLabel, pointsWonTeamLabel1,
                                                  pointsWonTeamLabel2, overallScoresCategoryLabel, player1OverallScoreLabel,
                                                  player2OverallScoreLabel, player3OverallScoreLabel, player4OverallScoreLabel})
        label->rescale();

    for (auto clickableCardArray : std::vector<ClickableCardArray *>{partnerCards})
        clickableCardArray->rescale();
}

void GameInfoWidget::refreshWidget(GameData &pData)
{
    updateBid(pData.roundInfo.bidPlayer, pData.roundInfo.bidAmount);
    updatePartner(pData.roundInfo.partnerCard, pData.roundInfo.partnerPlayerNum);
    updateTrump(pData.roundInfo.trump);
    updatePointsMiddle(pData.roundInfo.pointsMiddle, pData.roundInfo.bidPlayer != PLAYER_UNDEFINED);
    updateTeams(pData.roundInfo.teams);
    updatePlayerPoints(pData.roundInfo.playerScores);
    updateTeamPoints(pData.roundInfo.teamScores, pData.roundInfo.teams);
    updateOverallScores(pData.overallInfo.playerScores);
}

void GameInfoWidget::updateBid(int pBidPlayerNum, int pBidAmount)
{
    if (data.bidPlayerNum != pBidPlayerNum || data.bidAmount != pBidAmount)
    {
        data.bidPlayerNum = pBidPlayerNum;
        data.bidAmount = pBidAmount;

        if (data.bidPlayerNum == PLAYER_UNDEFINED) // bid is not known yet
        {
            bidPlayerLabel->setText("???");
            bidAmountLabel->setText("???");
        }
        else
        {
            auto playerNames = Settings::Appearance::readPlayerNames();
            bidPlayerLabel->setText(QString::fromStdString(playerNames[data.bidPlayerNum]));
            bidAmountLabel->setText(QString::number(data.bidAmount));
        }
    }
}

void GameInfoWidget::updatePartner(Card pPartnerCard, int pPartnerPlayerNum)
{
    if (data.partnerCard != pPartnerCard)
    {
        data.partnerCard = pPartnerCard;
        data.partnerPlayerNum = pPartnerPlayerNum;
    }

    // always show partner card (we want to show blank image if partner not known)
    partnerCards->showCards({data.partnerCard});
}

void GameInfoWidget::updateTrump(int pTrumpSuit)
{
    if (data.trumpSuit != pTrumpSuit)
    {
        data.trumpSuit = pTrumpSuit;

        switch (data.trumpSuit)
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
}

void GameInfoWidget::updatePointsMiddle(int pPointsMiddle, bool pPointsMiddleKnown)
{
    if (data.pointsMiddle != pPointsMiddle || data.pointsMiddleKnown != pPointsMiddleKnown)
    {
        data.pointsMiddle = pPointsMiddle;
        data.pointsMiddleKnown = pPointsMiddleKnown;

        std::string text = data.pointsMiddleKnown ? std::to_string(data.pointsMiddle) : "???";
        pointsMiddleLabel->setText(QString::fromStdString(text));
    }
}

void GameInfoWidget::updateTeams(std::array<Team, 2> &pTeams)
{
    if (data.teams != pTeams)
    {
        data.teams = pTeams;

        for (auto teamNum : std::vector<int>{TEAM_1, TEAM_2})
        {
            std::string text = data.teams[teamNum].getTeamName();
            getTeamLabel(teamNum)->setText(QString::fromStdString(text));
        }
    }
}

void GameInfoWidget::updateOverallScores(const std::map<int, int> &pOverallPlayerScores)
{
    if (data.overallPlayerScores != pOverallPlayerScores)
    {
        data.overallPlayerScores = pOverallPlayerScores;

        auto playerNames = Settings::Appearance::readPlayerNames();

        for (auto playerNum : std::vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
        {
            std::string text = playerNames[playerNum] + ": " + std::to_string(data.overallPlayerScores[playerNum]);
            getPlayerOverallScoreLabel(playerNum)->setText(QString::fromStdString(text));
        }
    }
}

void GameInfoWidget::updatePlayerPoints(const std::map<int, int> &pPlayerScores)
{
    if (data.playerScores != pPlayerScores)
    {
        data.playerScores = pPlayerScores;

        std::vector<std::pair<int, int>> sortedPlayerScores;

        for (auto it = data.playerScores.begin(); it != data.playerScores.end(); it++)
        {
            sortedPlayerScores.push_back(std::make_pair(it->first, it->second));
        }

        std::sort(sortedPlayerScores.begin(), sortedPlayerScores.end(), ScoreCompare());

        auto playerNames = Settings::Appearance::readPlayerNames();

        for (auto it : sortedPlayerScores)
        {
            int playerNum = it.first;
            int score = it.second;
            std::string text = playerNames[playerNum] + ": " + std::to_string(score);
            getPointsWonPlayerLabel(playerNum)->setText(QString::fromStdString(text));
        }
    }
}

void GameInfoWidget::updateTeamPoints(const std::map<int, int> &pTeamScores, const std::array<Team, 2> &pTeams)
{
    if (data.teamScores != pTeamScores) // team names are updated in "updateTeam1", "updateTeam2"
    {
        data.teamScores = pTeamScores;

        std::vector<std::pair<int, int>> sortedTeamScores;

        for (auto it = data.teamScores.begin(); it != data.teamScores.end(); it++)
        {
            sortedTeamScores.push_back(std::make_pair(it->first, it->second));
        }

        std::sort(sortedTeamScores.begin(), sortedTeamScores.end(), ScoreCompare());

        for (auto it : sortedTeamScores)
        {
            int teamNum = it.first;
            int score = it.second;

            std::string text = (pTeams[teamNum].empty()) ? "???" : pTeams[teamNum].getTeamName() + ": " + std::to_string(score);
            getPointsWonTeamLabel(teamNum)->setText(QString::fromStdString(text));
        }
    }
}

QLabel *GameInfoWidget::getTeamLabel(int teamNum)
{
    switch (teamNum)
    {
    case TEAM_1:
        return team1Label;
    case TEAM_2:
        return team2Label;
    default:
        return nullptr;
    }
}

QLabel *GameInfoWidget::getPointsWonPlayerLabel(int playerNum)
{
    switch (playerNum)
    {
    case PLAYER_1:
        return pointsWonPlayerLabel1;
    case PLAYER_2:
        return pointsWonPlayerLabel2;
    case PLAYER_3:
        return pointsWonPlayerLabel3;
    case PLAYER_4:
        return pointsWonPlayerLabel4;
    default:
        return nullptr;
    }
}

QLabel *GameInfoWidget::getPlayerOverallScoreLabel(int playerNum)
{
    switch (playerNum)
    {
    case PLAYER_1:
        return player1OverallScoreLabel;
    case PLAYER_2:
        return player2OverallScoreLabel;
    case PLAYER_3:
        return player3OverallScoreLabel;
    case PLAYER_4:
        return player4OverallScoreLabel;
    default:
        return nullptr;
    }
}

QLabel *GameInfoWidget::getPointsWonTeamLabel(int teamNum)
{
    switch (teamNum)
    {
    case TEAM_1:
        return pointsWonTeamLabel1;
    case TEAM_2:
        return pointsWonTeamLabel2;
    default:
        return nullptr;
    }
}
