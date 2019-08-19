#include <algorithm>
#include <QApplication>
#include <QDesktopWidget>
#include <QProgressBar>
#include <QRect>
#include <QSize>
#include <QThread>
#include <string>
#include <vector>

#include "bidDialog.h"
#include "card.h"
#include "clickableCard.h"
#include "deck.h"
#include "gameData.h"
#include "gameDatabase.h"
#include "gameInfoWidget.h"
#include "mainWindow.h"
#include "messageBox.h"
#include "middleDialog.h"
#include "preferencesDialog.h"
#include "saveSlotDialog.h"
#include "scoresDialog.h"
#include "settings.h"
#include "utils.h"

using namespace std;

void MainWindow::setupActions()
{
    newGameAction = new QAction(QMenu::tr("&New Game"), this);
    newGameAction->setShortcuts(QKeySequence::New);
    newGameAction->setStatusTip("Start a new game");
    QObject::connect(newGameAction, &QAction::triggered, this, &MainWindow::onNewGameAction);

    saveGameAction = new QAction(QMenu::tr("&Save Game"), this);
    saveGameAction->setShortcuts(QKeySequence::Save);
    saveGameAction->setStatusTip("Save the current game");
    QObject::connect(saveGameAction, &QAction::triggered, this, &MainWindow::onSaveGameAction);

    loadGameAction = new QAction(QMenu::tr("&Load Game"), this);
    loadGameAction->setShortcuts(QKeySequence::Open);
    loadGameAction->setStatusTip("Load an existing game");
    QObject::connect(loadGameAction, &QAction::triggered, this, &MainWindow::onLoadGameAction);

    exitMainMenuAction = new QAction(QMenu::tr("Exit to Main Menu"), this);
    exitMainMenuAction->setStatusTip("Exit to the game menu");
    QObject::connect(exitMainMenuAction, &QAction::triggered, this, &MainWindow::onExitMainMenuAction);

    closeAction = new QAction(QMenu::tr("&Close Game"), this);
    closeAction->setShortcuts(QKeySequence::Quit);
    closeAction->setStatusTip("Close the application");
    QObject::connect(closeAction, &QAction::triggered, this, &MainWindow::onCloseAction);

    preferencesAction = new QAction(QMenu::tr("&Preferences"), this);
    preferencesAction->setShortcuts(QKeySequence::Preferences);
    preferencesAction->setStatusTip("Edit the preferences");
    QObject::connect(preferencesAction, &QAction::triggered, this, &MainWindow::onPreferencesAction);

    viewScoresAction = new QAction(QMenu::tr("View scores"), this);
    viewScoresAction->setStatusTip("View scores for current game");
    QObject::connect(viewScoresAction, &QAction::triggered, this, &MainWindow::onViewScoresAction);

    aboutAction = new QAction(QMenu::tr("About"), this);
    aboutAction->setStatusTip("Info about the game");
    QObject::connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutAction);
}

void MainWindow::setupMenus()
{
    menuBar()->setStyleSheet("background-color: white");

    fileMenu = menuBar()->addMenu(QMenu::tr("&File"));
    fileMenu->addAction(newGameAction);
    fileMenu->addAction(saveGameAction);
    fileMenu->addAction(loadGameAction);
    fileMenu->addAction(exitMainMenuAction);
    fileMenu->addAction(closeAction);
    fileMenu->setStyleSheet("QMenu::item:selected { background-color: rgb(135, 206, 250); color: rgb(0, 0, 0);}"
                            "QMenu::item { background-color: white; }");

    editMenu = menuBar()->addMenu(QMenu::tr("&Edit"));
    editMenu->addAction(preferencesAction);
    editMenu->setStyleSheet("QMenu::item:selected { background-color: rgb(135, 206, 250); color: rgb(0, 0, 0);}"
                            "QMenu::item { background-color: white; }");

    gameMenu = menuBar()->addMenu(QMenu::tr("&Game"));
    gameMenu->addAction(viewScoresAction);
    gameMenu->setStyleSheet("QMenu::item:selected { background-color: rgb(135, 206, 250); color: rgb(0, 0, 0);}"
                            "QMenu::item { background-color: white; }");

    helpMenu = menuBar()->addMenu(QMenu::tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->setStyleSheet("QMenu::item:selected { background-color: rgb(135, 206, 250); color: rgb(0, 0, 0);}"
                            "QMenu::item { background-color: white; }");
}

MainWindow::MainWindow(QWidget *parent) : ScaledQMainWindow(parent)
{
    setupActions();
    setupMenus();

    widget = new MainWidget(this);
    widget->setParent(this);
    widget->refreshInfoWidget(gamedata);

    refreshNameTags(Settings::Appearance::readShowNameTags());

    progressBar = new ScaledQProgressBar;
    progressBar->setParent(this);
    progressBar->move({500, 250});
    progressBar->resize(300, 30);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setTextVisible(true);
    progressBar->setStyleSheet("background-color: white");
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->hide();

    setCentralWidget(widget);
    setWindowTitle("Rook");
    setWindowIcon(QIcon(":rookicon.gif"));
    resize(MAIN_WINDOW_SIZE);
    #ifdef WINDOW_ALWAYS_ON_TOP
    setWindowFlags(Qt::WindowStaysOnTopHint);
    #endif

    QPixmap bkgnd(":background.PNG");
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    setPalette(palette);
}

void MainWindow::rescale() // update resolution
{
    updateScaleFactor();
    setGeometry(geometry());

    widget->rescale();
    progressBar->rescale();

    Utils::Ui::moveWindowToCenter(this, -36);
}

void MainWindow::refreshNameTags(bool showNameTags)
{
    widget->refreshNameTags(showNameTags);
}

void MainWindow::onNewGameAction()
{
    gamedata.clearOverallInfo();
    showNewGameMessage();
    startNewRound();
}

void MainWindow::onLoadGameAction()
{
    int slotSelected = SLOT_UNDEFINED;

    SaveSlotDialog saveGameDlg(slotSelected);
    Utils::Ui::moveParentlessDialog(&saveGameDlg, this, DIALOG_POSITION_CENTER);

    if (!saveGameDlg.exec())
    {
        return;
    }

    QString dbName = getSlotDbName(slotSelected);
    db.LoadFromDb(dbName, gamedata);

    widget->refreshCardWidgets(gamedata);
    widget->refreshInfoWidget(gamedata);
    widget->setMenuWidgetVisible(!hasRoundStarted());

    showLoadGameMessage();
}

void MainWindow::onSaveGameAction()
{
    int slotSelected = SLOT_UNDEFINED;

    SaveSlotDialog saveGameDlg(slotSelected);
    Utils::Ui::moveParentlessDialog(&saveGameDlg, this, DIALOG_POSITION_CENTER);

    if (!saveGameDlg.exec())
    {
        return;
    }

    progressBar->setFormat("Saving Game...");
    progressBar->showNormal();
    progressBar->setValue(0);

    QString dbName = getSlotDbName(slotSelected);
    db.SaveToDb(dbName, gamedata, progressBar);

    progressBar->hide();

    showSaveGameMessage();
}

void MainWindow::onExitMainMenuAction()
{
    showExitMainMenuMessage();

    gamedata.clear();

    widget->refreshCardWidgets(gamedata);
    widget->refreshInfoWidget(gamedata);
    widget->setMenuWidgetVisible(true);
}

void MainWindow::onPreferencesAction()
{
    PreferencesDialog preferencesDlg(this);
    Utils::Ui::moveWindowToCenter(&preferencesDlg);
    auto result = preferencesDlg.exec();

    // appearance dialog may have changed player names in info widget
    widget->refreshInfoWidget(gamedata);
}

void MainWindow::onCloseAction()
{
    this->close();
}

void MainWindow::onViewScoresAction()
{
    ScoresDialog scoresDlg;
    scoresDlg.setupTableWidget(gamedata.scoreHistory);
    Utils::Ui::moveParentlessDialog(&scoresDlg, this, DIALOG_POSITION_CENTER);
    auto result = scoresDlg.exec();
}

void MainWindow::onAboutAction()
{
    showAboutMessage();
}

// very sequential function - order matters
void MainWindow::startNewRound()
{
    gamedata.clearRoundSpecificInfo();
    gamedata.overallInfo.roundNum += 1;

    widget->refreshCardWidgets(gamedata);
    widget->refreshInfoWidget(gamedata);
    widget->setMenuWidgetVisible(false);

    showNewRoundMessage();

    Deck deck;
    deck.initialize();
    deck.deal(gamedata.playerArr, gamedata.nest);

    // refresh player 1 cards
    widget->refreshCardWidgets(gamedata);

    BidDialog bidDlg(this);
    Utils::Ui::moveParentlessDialog(&bidDlg, this, DIALOG_POSITION_CENTER);
    auto player1WonBid = bidDlg.exec();

    // refresh bid
    widget->refreshInfoWidget(gamedata);

    if (player1WonBid)
    {
        MiddleDialog middleDlg(gamedata.roundInfo.trump, gamedata.roundInfo.partnerCard, widget, this);
        Utils::Ui::moveParentlessDialog(&middleDlg, this, DIALOG_POSITION_MIDDLE_DLG);

        if (!middleDlg.exec())
        {
            qFatal("Problem executing middle dialog");
            return;
        }
    }

    setRookSuitToTrump();

    if (gamedata.playerArr[PLAYER_1].cardArr.hasCard({gamedata.roundInfo.trump, VALUE_ROOK}))
    {
        // re-sort and redraw player 1 cards
        gamedata.playerArr[PLAYER_1].cardArr.sort(gamedata.roundInfo.trump);
        widget->refreshCardWidgets(gamedata);
    }

    gamedata.roundInfo.pointsMiddle = gamedata.nest.getNumPoints();

    // refresh trump, partner, points middle
    widget->refreshInfoWidget(gamedata);

    showGameStartingMessage();

    // play first few cards if necessary
    // for rest of round, play will resume when user clicks a card
    // see MainWidget::onCardClicked
    widget->startNewHand(gamedata.roundInfo.bidPlayer);
}

void MainWindow::showNewGameMessage()
{
    MessageBox msgBox;
    msgBox.setText("Clearing previous scores and starting new game...");
    msgBox.setWindowTitle("New game");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showSaveGameMessage()
{
    MessageBox msgBox;
    msgBox.setText("Saved Game");
    msgBox.setWindowTitle("Save Game");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showLoadGameMessage()
{
    MessageBox msgBox;
    msgBox.setText("Loaded Game");
    msgBox.setWindowTitle("Load Game");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showExitMainMenuMessage()
{
    MessageBox msgBox;
    msgBox.setWindowTitle("Exit to Main Menu");
    msgBox.setText("Any unsaved progress will be lost.\n\nExiting to main menu...");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showNewRoundMessage()
{
    MessageBox msgBox;
    msgBox.setText("A new round is starting...");
    msgBox.setWindowTitle("New round");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showGameStartingMessage()
{
    MessageBox msgBox;
    msgBox.setText("Trump, partner, points in middle updated.\n\nGame starting.");
    msgBox.setWindowTitle("Start Game");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showAboutMessage()
{
    MessageBox msgBox;
    msgBox.setText("<p>Rook game made by Nathan Esau<br /><br />"
                   "<a href=\"https://www.github.com/nathanesau/RookGame\">https://www.github.com/nathanesau/RookGame</a></p>");
    msgBox.setWindowTitle("About Rook");
    msgBox.turnOnHyperLinks();
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

bool MainWindow::hasRoundStarted() const
{
    // if player 1 cards haven't been dealt yet, that round hasn't started yet
    return !gamedata.playerArr[PLAYER_1].cardArr.empty();
}

void MainWindow::setRookSuitToTrump()
{
    for (auto playerNum : vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        for (auto &card : gamedata.playerArr[playerNum].cardArr)
        {
            if (card.suit == SUIT_SPECIAL)
            {
                card.suit = gamedata.roundInfo.trump;
                return;
            }
        }
    }    
}

QString MainWindow::getSlotDbName(int slotNumber) const
{
    switch (slotNumber)
    {
    case SLOT_1:
        return "slot1.db";
    case SLOT_2:
        return "slot2.db";
        break;
    default:
        return "slot3.db";
    }
}