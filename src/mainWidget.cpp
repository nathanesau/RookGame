#include <QThread>
#include <set>
#include <string>

#include "cpuPlayer.h"
#include "gameData.h"
#include "mainWidget.h"
#include "mainWindow.h"
#include "messageBox.h"
#include "roundSummaryDialog.h"
#include "settings.h"
#include "utils.h"

using namespace std;

MainWidgetData::MainWidgetData()
{
}

MainWidget::MainWidget(MainWindow *pMainWindow, QWidget *parent) : mainWindow(pMainWindow),
                                                                   QDialogWithClickableCardArray(false, parent)
{
    auto setupLabel = [this](QLabel *label, QString text, QPoint pos, QSize size) {
        label->setParent(this);
        label->setFont(QFont("Times", 11));
        label->setText(text);
        label->move(pos);
        label->resize(size);
        label->setStyleSheet("background-color: white");
        label->setAlignment(Qt::AlignCenter);
    };

    player1Cards = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_BOTTOM, SIZE_NORMAL, this);
    #ifdef CPU_DEBUG
    player2Cards = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_LEFT, SIZE_SMALL, this);
    player3Cards = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_TOP, SIZE_SMALL, this);
    player4Cards = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_RIGHT, SIZE_SMALL, this);
    #endif

    player1CardPlayed = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_CENTER_BOTTOM, SIZE_NORMAL, this);
    player2CardPlayed = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_CENTER_LEFT, SIZE_NORMAL, this);
    player3CardPlayed = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_CENTER_TOP, SIZE_NORMAL, this);
    player4CardPlayed = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_CENTER_RIGHT, SIZE_NORMAL, this);

    auto playerNames = Settings::Appearance::readPlayerNames();

    player1NameLabel = new ScaledQLabel;
    setupLabel(player1NameLabel, QString::fromStdString(playerNames[PLAYER_1]), {550, 800}, {75, 25});

    player2NameLabel = new ScaledQLabel;
    setupLabel(player2NameLabel, QString::fromStdString(playerNames[PLAYER_2]), {25, 425}, {75, 25});

    player3NameLabel = new ScaledQLabel;
    setupLabel(player3NameLabel, QString::fromStdString(playerNames[PLAYER_3]), {550, 140}, {75, 25});

    player4NameLabel = new ScaledQLabel;
    setupLabel(player4NameLabel, QString::fromStdString(playerNames[PLAYER_4]), {1100, 425}, {75, 25});

    infoWidget = new GameInfoWidget(pMainWindow);
    infoWidget->setParent(this);
    infoWidget->move(QPoint(0, 0));

    menuWidget = new GameMenuWidget(pMainWindow);
    menuWidget->setParent(this);
    menuWidget->move(QPoint(400, 300));

    resize(MAIN_WIDGET_SIZE);
    // no window title, etc.
}

void MainWidget::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto clickableCardArray : vector<ClickableCardArray *>{player1CardPlayed, player2CardPlayed,
                                                                player3CardPlayed, player4CardPlayed, player1Cards
                                                                #ifdef CPU_DEBUG
                                                                ,player2Cards, player3Cards, player4Cards
                                                                #endif
                                                                })
        clickableCardArray->rescale();

    for (auto label : vector<ScaledQLabel *>{player1NameLabel, player2NameLabel,
                                             player3NameLabel, player4NameLabel})
        label->rescale();

    for (auto widget : vector<GameInfoWidget *>{infoWidget})
        widget->rescale();

    for (auto widget : vector<GameMenuWidget *>{menuWidget})
        widget->rescale();
}

void MainWidget::finishExistingHand(Card player1Card)
{
    playCard(player1Card, PLAYER_1);

    player1Cards->showCards(gamedata.playerArr[PLAYER_1].cardArr);

    showCardPlayed(player1Card, PLAYER_1);

    for (auto playerNum : vector<int>{PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if (gamedata.handInfo.cardPlayed[playerNum] == Card(SUIT_UNDEFINED, VALUE_UNDEFINED)) // cpu hasn't played yet
        {
            playCard(cpu.getCardToPlay(playerNum), playerNum);

            #ifdef CPU_DEBUG
            player2Cards->showCards(gamedata.playerArr[PLAYER_2].cardArr);
            player3Cards->showCards(gamedata.playerArr[PLAYER_3].cardArr);
            player4Cards->showCards(gamedata.playerArr[PLAYER_4].cardArr);
            #endif

            showCardPlayed(gamedata.handInfo.cardPlayed[playerNum], playerNum);
        }
    }

    gamedata.roundInfo.updateScores(gamedata.handInfo);

    showPartnerCardIfApplicable();

    // refresh playerScores, teamScores
    infoWidget->refreshWidget(gamedata);

    showHandResult();

    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        getCardPlayedWidget(playerNum)->hideCards();
    }
}

void MainWidget::startNewHand(int startingPlayerNum)
{
    if (startingPlayerNum != PLAYER_1)
    {
        // sleep before starting hand
        repaint();
        QThread::msleep(500);
    }

    gamedata.handInfo.clear();

    int playerNum = startingPlayerNum;

    while (playerNum != PLAYER_1)
    {
        playCard(cpu.getCardToPlay(playerNum), playerNum);

        showCardPlayed(gamedata.handInfo.cardPlayed[playerNum], playerNum);

        playerNum = gamedata.playerArr[playerNum].getNextPlayerNum();
    }

    // wait for card click to finish hand
}

bool MainWidget::validateCard(ClickableCard *clickableCard)
{
    CardVector playableCards = gamedata.playerArr[PLAYER_1].cardArr.getPlayableCards(gamedata.handInfo);

    if (!playableCards.hasCard(clickableCard->data)) // invalid card
    {
        MessageBox msgBox;
        msgBox.showCards({clickableCard->data});
        msgBox.setText("Invalid card clicked. Must follow suit.");
        msgBox.setWindowTitle("Invalid card");
        Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
        msgBox.exec();

        return false;
    }

    return true;
}

void MainWidget::onCardClicked(ClickableCard *clickableCard)
{
    #ifdef CPU_DEBUG
    if (!gamedata.playerArr[PLAYER_1].cardArr.hasCard(clickableCard->data))
    {
        return;
    }
    #endif

    if (!validateCard(clickableCard))
    {
        return;
    }

    finishExistingHand(clickableCard->data);

    if (isRoundOver())
    {
        gamedata.roundInfo.addPointsMiddleToScores(gamedata.handInfo);

        // refresh playerScores, teamScores
        infoWidget->refreshWidget(gamedata);

        showNestResult();

        gamedata.overallInfo.updatePlayerScores(gamedata.roundInfo);

        // refresh overallScores
        infoWidget->refreshWidget(gamedata);

        RoundSummaryDialog summaryDlg;
        summaryDlg.updateScores(gamedata.roundInfo.getRoundScores());
        Utils::Ui::moveParentlessDialog(&summaryDlg, mainWindow, DIALOG_POSITION_CENTER);

        if (!summaryDlg.exec())
        {
            qFatal("Problem executing round summary dialog");
            return;
        }

        gamedata.scoreHistory[gamedata.overallInfo.roundNum] = gamedata.roundInfo.getRoundScores();
        gamedata.clearRoundSpecificInfo();

        infoWidget->refreshWidget(gamedata);

        // show game menu
        menuWidget->show();
    }
    else
    {
        startNewHand(gamedata.handInfo.getWinningPlayerNum(gamedata.roundInfo));
    }
}

void MainWidget::onCardHoverEnter(ClickableCard *clickableCard)
{
    // do nothing
}

void MainWidget::onCardHoverLeave(ClickableCard *clickableCard)
{
    // do nothing
}

void MainWidget::refreshCardWidgets(GameData &pData)
{
    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if (data.cardPlayed[playerNum] != pData.handInfo.cardPlayed[playerNum])
        {
            data.cardPlayed[playerNum] = pData.handInfo.cardPlayed[playerNum];

            CardVector cardsToDisplay;

            if (data.cardPlayed[playerNum] != Card(SUIT_UNDEFINED, VALUE_UNDEFINED))
            {
                cardsToDisplay.push_back({data.cardPlayed[playerNum]});
            }

            getCardPlayedWidget(playerNum)->showCards(cardsToDisplay);
        }
    }

    auto areCardVectorsEqual = [](CardVector &cardArr1, CardVector &cardArr2) {
        if (cardArr1.size() != cardArr2.size())
        {
            return false;
        }

        for(int i = 0; i < cardArr1.size(); i++)
        {
            if(cardArr1[i] != cardArr2[i])
            {
                return false;
            }
        }

        return true;
    };

    if (!areCardVectorsEqual(data.player1Cards, pData.playerArr[PLAYER_1].cardArr))
    {
        data.player1Cards = pData.playerArr[PLAYER_1].cardArr;
        player1Cards->showCards(data.player1Cards);
    }

    #ifdef CPU_DEBUG
    if (!areCardVectorsEqual(data.player2Cards, pData.playerArr[PLAYER_2].cardArr))
    {
        data.player2Cards = pData.playerArr[PLAYER_2].cardArr;
        player2Cards->showCards(data.player2Cards);
    }

    if (!areCardVectorsEqual(data.player3Cards, pData.playerArr[PLAYER_3].cardArr))
    {
        data.player3Cards = pData.playerArr[PLAYER_3].cardArr;
        player3Cards->showCards(data.player3Cards);
    }

    if (!areCardVectorsEqual(data.player4Cards, pData.playerArr[PLAYER_4].cardArr))
    {
        data.player4Cards = pData.playerArr[PLAYER_4].cardArr;
        player4Cards->showCards(data.player4Cards);
    }
    #endif
}

void MainWidget::refreshInfoWidget(GameData &pData)
{
    infoWidget->refreshWidget(pData);
}

void MainWidget::setMenuWidgetVisible(bool visible)
{
    menuWidget->setVisible(visible);
}

void MainWidget::refreshNameTags(bool showNameTags)
{
    auto playerNames = Settings::Appearance::readPlayerNames();

    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        auto label = getPlayerNameLabel(playerNum);
        label->setText(QString::fromStdString(playerNames[playerNum]));
        label->setVisible(showNameTags);
    }
}

void MainWidget::showCardPlayed(const Card &card, int playerNum)
{
    getCardPlayedWidget(playerNum)->showCards({card});

    bool sleep = []() {
        return true; // sleep after any card is played
    }();

    if (sleep)
    {
        repaint();
        QThread::msleep(500);
    }
}

void MainWidget::showPartnerCardIfApplicable()
{
    for (auto it = gamedata.handInfo.cardPlayed.begin(); it != gamedata.handInfo.cardPlayed.end(); it++)
    {
        auto currentCard = (*it).second;

        if (currentCard == gamedata.roundInfo.partnerCard)
        {
            // refresh partner card, teams
            infoWidget->refreshWidget(gamedata);

            string msg = gamedata.playerArr[gamedata.roundInfo.partnerPlayerNum].getPlayerName() + " is the partner. Teams updated.";

            MessageBox msgBox;
            msgBox.showCards({gamedata.roundInfo.partnerCard});
            msgBox.setText(QString::fromStdString(msg));
            msgBox.setWindowTitle("Partner card");
            Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
            msgBox.exec();

            return;
        }
    }
}

void MainWidget::showHandResult()
{
    auto msg = []() -> string {
        return gamedata.playerArr[gamedata.handInfo.getWinningPlayerNum(gamedata.roundInfo)].getPlayerName() +
               " won the hand for " + to_string(gamedata.handInfo.points) + " points with the";
    }();

    MessageBox msgBox;
    msgBox.showCards({gamedata.handInfo.getWinningCard(gamedata.roundInfo)});
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setWindowTitle("Hand result");
    msgBox.move({340, 250});
    Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWidget::showNestResult()
{
    string msg = gamedata.playerArr[gamedata.handInfo.getWinningPlayerNum(gamedata.roundInfo)].getPlayerName() +
                 " won the nest. Nest had " + to_string(gamedata.roundInfo.pointsMiddle) + " points.";

    MessageBox msgBox;
    msgBox.changeCardArrayDrawPosition(DRAW_POSITION_MESSAGE_BOX_NEST);
    msgBox.showCards(gamedata.nest);
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setWindowTitle("Last hand");
    Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

ClickableCardArray *MainWidget::getCardPlayedWidget(int playerNum)
{
    switch (playerNum)
    {
    case PLAYER_1:
        return player1CardPlayed;
    case PLAYER_2:
        return player2CardPlayed;
    case PLAYER_3:
        return player3CardPlayed;
    case PLAYER_4:
        return player4CardPlayed;
    default:
        return nullptr;
    }
}

QLabel *MainWidget::getPlayerNameLabel(int playerNum)
{
    switch (playerNum)
    {
    case PLAYER_1:
        return player1NameLabel;
    case PLAYER_2:
        return player2NameLabel;
    case PLAYER_3:
        return player3NameLabel;
    case PLAYER_4:
        return player4NameLabel;
    default:
        return nullptr;
    }
}

bool MainWidget::isRoundOver()
{
    return std::all_of(gamedata.playerArr.begin(), gamedata.playerArr.end(), [](auto &p) { return p.cardArr.empty(); });
}

void MainWidget::playCard(Card cardPlayed, int playerNum)
{
    if (gamedata.handInfo.startingPlayerNum == PLAYER_UNDEFINED) // first card played
    {
        gamedata.handInfo.startingPlayerNum = playerNum;
        gamedata.handInfo.suit = cardPlayed.suit;
    }

    gamedata.handInfo.cardPlayed[playerNum] = cardPlayed;
    gamedata.handInfo.points += cardPlayed.getPointValue();

    if (cardPlayed == gamedata.roundInfo.partnerCard) // update partner and teams
    {
        gamedata.roundInfo.partnerPlayerNum = playerNum;
        gamedata.roundInfo.updateTeams();
    }

    gamedata.playerArr[playerNum].cardArr.remove({cardPlayed});
}