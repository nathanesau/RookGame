#include <QThread>
#include <set>
#include <string>

#include "cpuPlayer.h"
#include "gameController.h"
#include "mainWidget.h"
#include "mainWindow.h"
#include "messageBox.h"
#include "roundSummaryDialog.h"
#include "settings.h"
#include "utils.h"

using namespace std;

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

    centerCards = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_CENTER, SIZE_NORMAL, this);
    bottomCards = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_BOTTOM, SIZE_NORMAL, this);
    
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

    updateNameTags(Settings::Appearance::readShowNameTags());

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
                                                                player3CardPlayed, player4CardPlayed,
                                                                centerCards, bottomCards})
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
    gc.playCard(player1Card, PLAYER_1);

    bottomCards->showCards(gc.data.playerArr[PLAYER_1].cardArr);

    showCardPlayed(player1Card, PLAYER_1);

    for (auto playerNum : vector<int>{PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if (gc.data.handInfo.cardPlayed[playerNum] == Card(SUIT_UNDEFINED, VALUE_UNDEFINED)) // cpu hasn't played yet
        {
            gc.playCard(cpu.getCardToPlay(playerNum), playerNum);

            showCardPlayed(gc.data.handInfo.cardPlayed[playerNum], playerNum);
        }
    }

    gc.data.roundInfo.updateScores(gc.data.handInfo);

    showPartnerCardIfApplicable();

    infoWidget->updatePlayerPoints(gc.data.roundInfo.playerScores);
    infoWidget->updateTeamPoints(gc.data.roundInfo.teamScores);

    showHandResult();

    player1CardPlayed->hideCards();
    player2CardPlayed->hideCards();
    player3CardPlayed->hideCards();
    player4CardPlayed->hideCards();
}

void MainWidget::startNewHand(int startingPlayerNum)
{
    if (startingPlayerNum != PLAYER_1)
    {
        // sleep before starting hand
        repaint();
        QThread::msleep(500);
    }

    gc.data.handInfo.clear();

    int playerNum = startingPlayerNum;

    while (playerNum != PLAYER_1)
    {
        gc.playCard(cpu.getCardToPlay(playerNum), playerNum);

        showCardPlayed(gc.data.handInfo.cardPlayed[playerNum], playerNum);

        playerNum = gc.data.playerArr[playerNum].getNextPlayerNum();
    }

    // wait for card click to finish hand
}

bool MainWidget::validateCard(ClickableCard *clickableCard)
{
    CardVector playableCards = gc.data.playerArr[PLAYER_1].cardArr.getPlayableCards(gc.data.handInfo);

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
    if (!validateCard(clickableCard))
    {
        return;
    }

    finishExistingHand(clickableCard->data);

    if (gc.isRoundOver())
    {
        gc.data.roundInfo.addPointsMiddleToScores(gc.data.handInfo);

        centerCards->showCards(gc.data.nest);

        infoWidget->updatePlayerPoints(gc.data.roundInfo.playerScores);
        infoWidget->updateTeamPoints(gc.data.roundInfo.teamScores);

        showNestResult();

        centerCards->hideCards();

        gc.data.overallInfo.updatePlayerScores(gc.data.roundInfo);

        infoWidget->updateOverallScores(gc.data.overallInfo.playerScores);

        RoundSummaryDialog summaryDlg;
        summaryDlg.updateScores(gc.data.roundInfo.getRoundScores());
        Utils::Ui::moveParentlessDialog(&summaryDlg, mainWindow, DIALOG_POSITION_CENTER);

        if (!summaryDlg.exec())
        {
            qFatal("Problem executing round summary dialog");
            return;
        }

        gc.data.scoreHistory[gc.data.overallInfo.roundNum] = gc.data.roundInfo.getRoundScores();
        gc.data.clearRoundSpecificInfo();

        infoWidget->resetRoundInfoToDefaults();

        // show game menu
        menuWidget->show();
    }
    else
    {
        startNewHand(gc.data.handInfo.getWinningPlayerNum(gc.data.roundInfo));
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

void MainWidget::showCardPlayed(const Card &card, int playerNum)
{
    switch (playerNum)
    {
    case PLAYER_1:
        player1CardPlayed->showCards({card});
        break;
    case PLAYER_2:
        player2CardPlayed->showCards({card});
        break;
    case PLAYER_3:
        player3CardPlayed->showCards({card});
        break;
    case PLAYER_4:
        player4CardPlayed->showCards({card});
        break;
    }

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
    for (auto it = gc.data.handInfo.cardPlayed.begin(); it != gc.data.handInfo.cardPlayed.end(); it++)
    {
        auto currentCard = (*it).second;

        if (currentCard == gc.data.roundInfo.partnerCard)
        {
            infoWidget->updatePartner(currentCard, gc.data.roundInfo.partnerPlayerNum);
            infoWidget->updateTeam1(gc.data.roundInfo.teams.first);
            infoWidget->updateTeam2(gc.data.roundInfo.teams.second);

            string msg = gc.data.playerArr[gc.data.roundInfo.partnerPlayerNum].getPlayerName() + " is the partner. Teams updated.";

            MessageBox msgBox;
            msgBox.showCards({gc.data.roundInfo.partnerCard});
            msgBox.setText(QString::fromStdString(msg));
            msgBox.setWindowTitle("Partner card");
            msgBox.resize({325, 250});
            Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
            msgBox.exec();

            return;
        }
    }
}

void MainWidget::showHandResult()
{
    auto msg = []() -> string {
        return gc.data.playerArr[gc.data.handInfo.getWinningPlayerNum(gc.data.roundInfo)].getPlayerName() +
               " won the hand for " + to_string(gc.data.handInfo.points) + " points with the";
    }();

    MessageBox msgBox;
    msgBox.showCards({gc.data.handInfo.getWinningCard(gc.data.roundInfo)});
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setWindowTitle("Hand result");
    msgBox.move({340, 250});
    Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWidget::showNestResult()
{
    string msg = gc.data.playerArr[gc.data.handInfo.getWinningPlayerNum(gc.data.roundInfo)].getPlayerName() +
                 " won the nest. Nest had " + to_string(gc.data.roundInfo.pointsMiddle) + " points.";

    MessageBox msgBox;
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setWindowTitle("Last hand");
    Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWidget::updateNameTags(bool showNameTags)
{
    if (showNameTags)
    {
        player1NameLabel->show();
        player2NameLabel->show();
        player3NameLabel->show();
        player4NameLabel->show();
    }
    else
    {
        player1NameLabel->hide();
        player2NameLabel->hide();
        player3NameLabel->hide();
        player4NameLabel->hide();
    }
}