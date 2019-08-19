#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <map>
#include <QFont>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
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

// implemented like a ScaledWidget
class PlayerNameLabel : public QWidget
{
    float prevScaleFactor;
    float scaleFactor;
    Qt::AlignmentFlag alignType; // only align left, align right, align top, align bottom supported

private:
    QString text;
    QFont font;

    // functions related to size or position
    void setFont(const QFont &pFont);

public:
    PlayerNameLabel(QWidget *parent = nullptr);
    void rescale();

    void setAlignType(Qt::AlignmentFlag pAlignType)
    {
        alignType = pAlignType;
    }

    // functions related to size or position
    void setGeometry(const QRect &rect);
    void resize(int w, int h);
    void move(const QPoint &pos);

    // functions to set widget data
    void setText(const QString &pText);

protected:
    // functions related to size or position
    void updateScaleFactor();

    // functions to set widget data
    void paintEvent(QPaintEvent *event) override;
};

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

    PlayerNameLabel *player1NameLabel;
    ClickableCardArray *player1CardPlayed;

    PlayerNameLabel *player2NameLabel;
    ClickableCardArray *player2CardPlayed;

    PlayerNameLabel *player3NameLabel;
    ClickableCardArray *player3CardPlayed;

    PlayerNameLabel *player4NameLabel;
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
    PlayerNameLabel *getPlayerNameLabel(int playerNum);
};

#endif