#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <map>
#include <QProgressBar>

#include "clickableCard.h"
#include "common.h"
#include "gameInfoWidget.h"
#include "gameMenuWidget.h"
#include "scaledWidgets.h"

using namespace std;

// forward declarations
class CpuDecisionMaker;
struct GameData;
class MainWindow;

// global declarations
extern CpuDecisionMaker cpu;
extern GameData gamedata;

const QSize MAIN_WIDGET_SIZE = {1200, 850};

struct MainWidgetData
{
    map<int, Card> cardPlayed;
    CardVector player1Cards;

    #ifdef CPU_DEBUG
    CardVector player2Cards;
    CardVector player3Cards;
    CardVector player4Cards;
    #endif

    MainWidgetData();
};

class MainWidget : public QDialogWithClickableCardArray
{
    MainWidgetData data;

private:
    MainWindow *mainWindow; // non-owning

    GameInfoWidget *infoWidget;
    GameMenuWidget *menuWidget;

    ScaledQLabel *player1NameLabel;
    ClickableCardArray *player1CardPlayed;

    ScaledQLabel *player2NameLabel;
    ClickableCardArray *player2CardPlayed;

    ScaledQLabel *player3NameLabel;
    ClickableCardArray *player3CardPlayed;

    ScaledQLabel *player4NameLabel;
    ClickableCardArray *player4CardPlayed;

    ClickableCardArray *player1Cards;

    #ifdef CPU_DEBUG
    ClickableCardArray *player2Cards;
    ClickableCardArray *player3Cards;
    ClickableCardArray *player4Cards;
    #endif

public:
    MainWidget(MainWindow *pMainWindow, QWidget *parent = nullptr);
    virtual void rescale();

    virtual void onCardClicked(ClickableCard *clickableCard);
    virtual void onCardHoverEnter(ClickableCard *clickableCard);
    virtual void onCardHoverLeave(ClickableCard *clickableCard);

    void refreshCardWidgets(GameData &pData);
    void refreshInfoWidget(GameData &pData);
    void setMenuWidgetVisible(bool visible);

    void refreshNameTags(bool showNameTags); // for apppearance page

    void startNewHand(int startingPlayerNum);

private:
    bool isRoundOver();
    void playCard(Card cardPlayed, int playerNum);

    bool validateCard(ClickableCard *clickableCard); // return true if valid

    void finishExistingHand(Card player1Card); // intentional copy

    void showCardPlayed(const Card &card, int playerNum);

    void showPartnerCardIfApplicable();
    void showHandResult();
    void showNestResult();

    ClickableCardArray *getCardPlayedWidget(int playerNum);
    QLabel *getPlayerNameLabel(int playerNum);
};

#endif