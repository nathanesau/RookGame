#ifndef GAMEINFOWIDGET_H
#define GAMEINFOWIDGET_H

#include "clickableCard.h"
#include "common.h"
#include "player.h"
#include "scaledWidgets.h"

// forward declarations
struct GameData;

struct ScoreCompare
{
    inline bool operator()(const std::pair<int, int> &score1, const std::pair<int, int> &score2)
    {
        if (score1.second < score2.second)
        {
            return false;
        }
        else if (score1.second == score2.second)
        {
            return score1.first < score2.first;
        }

        return true;
    }
};

const QSize GAME_INFO_WIDGET_SIZE = {1200, 130};

struct GameInfoWidgetData
{
    int bidPlayerNum;
    int bidAmount;
    Card partnerCard;
    int partnerPlayerNum;
    int trumpSuit;
    int pointsMiddle;
    bool pointsMiddleKnown;
    std::array<Team, 2> teams;
    std::map<int, int> playerScores;
    std::map<int, int> teamScores;
    std::map<int, int> overallPlayerScores;

    GameInfoWidgetData();
};

struct GameInfoWidget : public QDialogWithClickableCardArray
{
    // data variables corresponding to what is shown in widget
    // these may differ from what is in "GameData"
    GameInfoWidgetData data;

private:
    QMainWindow *mainWindow; // non-owning

    ScaledQLabel *bidCategoryLabel;
    ScaledQLabel *bidPlayerLabel;
    ScaledQLabel *bidAmountLabel;

    ScaledQLabel *partnerCardCategoryLabel;
    ClickableCardArray *partnerCards;

    ScaledQLabel *trumpCategoryLabel;
    ScaledQLabel *trumpLabel;

    ScaledQLabel *pointsMiddleCategoryLabel;
    ScaledQLabel *pointsMiddleLabel;

    ScaledQLabel *teamsCategoryLabel;
    ScaledQLabel *team1Label;
    ScaledQLabel *team2Label;

    ScaledQLabel *pointsWonPlayerCategoryLabel;
    ScaledQLabel *pointsWonPlayerLabel1;
    ScaledQLabel *pointsWonPlayerLabel2;
    ScaledQLabel *pointsWonPlayerLabel3;
    ScaledQLabel *pointsWonPlayerLabel4;

    ScaledQLabel *pointsWonTeamCategoryLabel;
    ScaledQLabel *pointsWonTeamLabel1;
    ScaledQLabel *pointsWonTeamLabel2;

    ScaledQLabel *overallScoresCategoryLabel;
    ScaledQLabel *player1OverallScoreLabel;
    ScaledQLabel *player2OverallScoreLabel;
    ScaledQLabel *player3OverallScoreLabel;
    ScaledQLabel *player4OverallScoreLabel;

public:
    GameInfoWidget(QMainWindow *pMainWindow, QWidget *parent = nullptr);
    virtual void rescale();

    virtual void onCardClicked(ClickableCard *clickableCard);
    virtual void onCardHoverEnter(ClickableCard *clickableCard);
    virtual void onCardHoverLeave(ClickableCard *clickableCard);

    void refreshWidget(GameData &pData);

private:
    void updateBid(int pBidPlayerNum, int pBidAmount);
    void updatePartner(Card pPartnerCard, int pPartnerPlayerNum = PLAYER_UNDEFINED);
    void updateTrump(int pTrumpSuit);
    void updatePointsMiddle(int pPointsMiddle, bool pPointsMiddleKnown);
    void updateTeams(std::array<Team, 2> &pTeams);
    void updatePlayerPoints(const std::map<int, int> &pPlayerScores);
    void updateTeamPoints(const std::map<int, int> &pTeamScores, const std::array<Team, 2> &pTeams);
    void updateOverallScores(const std::map<int, int> &pOverallPlayerScores);

    QLabel *getTeamLabel(int teamNum);
    QLabel *getPointsWonPlayerLabel(int playerNum);
    QLabel *getPointsWonTeamLabel(int teamNum);
    QLabel *getPlayerOverallScoreLabel(int playerNum);
};

#endif