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
#include "nestDialog.h"
#include "partnerDialog.h"
#include "preferencesDialog.h"
#include "roundSummaryDialog.h"
#include "saveSlotDialog.h"
#include "scoresDialog.h"
#include "settings.h"
#include "trumpDialog.h"
#include "utils.h"

void MainWindow::setupActions()
{
    newGameAction = new QAction(QMenu::tr("&New Game"), this);
    newGameAction->setShortcuts(QKeySequence::New);
    newGameAction->setStatusTip("Start a new game");
    connect(newGameAction, &QAction::triggered, this, &MainWindow::onNewGameAction);

    saveGameAction = new QAction(QMenu::tr("&Save Game"), this);
    saveGameAction->setShortcuts(QKeySequence::Save);
    saveGameAction->setStatusTip("Save the current game");
    connect(saveGameAction, &QAction::triggered, this, &MainWindow::onSaveGameAction);

    loadGameAction = new QAction(QMenu::tr("&Load Game"), this);
    loadGameAction->setShortcuts(QKeySequence::Open);
    loadGameAction->setStatusTip("Load an existing game");
    connect(loadGameAction, &QAction::triggered, this, &MainWindow::onLoadGameAction);

    exitMainMenuAction = new QAction(QMenu::tr("Exit to Main Menu"), this);
    exitMainMenuAction->setStatusTip("Exit to the game menu");
    connect(exitMainMenuAction, &QAction::triggered, this, &MainWindow::onExitMainMenuAction);

    closeAction = new QAction(QMenu::tr("&Close Game"), this);
    closeAction->setShortcuts(QKeySequence::Quit);
    closeAction->setStatusTip("Close the application");
    connect(closeAction, &QAction::triggered, this, &MainWindow::onCloseAction);

    preferencesAction = new QAction(QMenu::tr("&Preferences"), this);
    preferencesAction->setShortcuts(QKeySequence::Preferences);
    preferencesAction->setStatusTip("Edit the preferences");
    connect(preferencesAction, &QAction::triggered, this, &MainWindow::onPreferencesAction);

    viewScoresAction = new QAction(QMenu::tr("View scores"), this);
    viewScoresAction->setStatusTip("View scores for current game");
    connect(viewScoresAction, &QAction::triggered, this, &MainWindow::onViewScoresAction);

    aboutAction = new QAction(QMenu::tr("About"), this);
    aboutAction->setStatusTip("Info about the game");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutAction);
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

    connect(widget, &MainWidget::newGameButtonPressed, this, &MainWindow::onNewGameAction);
    connect(widget, &MainWidget::newRoundButtonPressed, this, &MainWindow::startNewRound);
    connect(widget, &MainWidget::saveGameButtonPressed, this, &MainWindow::onSaveGameAction);
    connect(widget, &MainWidget::loadGameButtonPressed, this, &MainWindow::onLoadGameAction);
    connect(widget, &MainWidget::loadGameButtonPressed, this, &MainWindow::close);
    connect(widget, &MainWidget::invalidCardPlayed, this, &MainWindow::showInvalidCardPlayedMessage);
    connect(widget, &MainWidget::roundSummary, this, &MainWindow::showRoundSummaryDialog);
    connect(widget, &MainWidget::partnerMessage, this, &MainWindow::showPartnerMessage);
    connect(widget, &MainWidget::handResultMessage, this, &MainWindow::showHandResultMessage);
    connect(widget, &MainWidget::nestResultMessage, this, &MainWindow::showNestResultMessage);

    refreshNameTags();

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

void MainWindow::refreshNameTags()
{
    bool showNameTags = Settings::Appearance::readShowNameTags();
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
    connect(&preferencesDlg, &PreferencesDialog::nameTagsChanged, this, &MainWindow::refreshNameTags);
    connect(&preferencesDlg, &PreferencesDialog::gameResolutionChanged, this, &MainWindow::rescale);

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

void MainWindow::showRoundSummaryDialog()
{
    RoundSummaryDialog summaryDlg;
    summaryDlg.updateScores(gamedata.roundInfo.getRoundScores());
    Utils::Ui::moveParentlessDialog(&summaryDlg, this, DIALOG_POSITION_CENTER);

    if (!summaryDlg.exec())
    {
        qFatal("Problem executing round summary dialog");
        return;
    }
}

void MainWindow::showNestDialog(const CardVector &originalNest)
{
    NestDialog nestDlg(originalNest);
    Utils::Ui::moveParentlessDialog(&nestDlg, this, DIALOG_POSITION_NEST_DLG);

    if (!nestDlg.exec())
    {
        qFatal("Problem executing nest dialog");
        return;
    }
}

void MainWindow::showPartnerDialog()
{
    PartnerDialog partnerDlg(gamedata.roundInfo.partnerCard);
    Utils::Ui::moveParentlessDialog(&partnerDlg, this, DIALOG_POSITION_PARTNER_DLG);

    if (!partnerDlg.exec())
    {
        qFatal("Problem executing partner dialog");
        return;
    }
}

void MainWindow::showTrumpDialog()
{
    TrumpDialog trumpDlg(gamedata.roundInfo.trump);
    Utils::Ui::moveParentlessDialog(&trumpDlg, this, DIALOG_POSITION_TRUMP_DLG);

    if (!trumpDlg.exec())
    {
        qFatal("Problem executing trump dialog");
        return;
    }
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

    BidDialog bidDlg;
    Utils::Ui::moveParentlessDialog(&bidDlg, this, DIALOG_POSITION_CENTER);
    auto player1WonBid = bidDlg.exec();

    showBidResultMessage();

    // refresh bid
    widget->refreshInfoWidget(gamedata);

    if (player1WonBid)
    {
        MiddleDialog middleDlg;

        connect(&middleDlg, &MiddleDialog::showInvalidMiddleDialogMessage, this, &MainWindow::showInvalidMiddleDialogMessage);
        connect(&middleDlg, &MiddleDialog::showNestDialog, this, &MainWindow::showNestDialog);
        connect(&middleDlg, &MiddleDialog::showPartnerDialog, this, &MainWindow::showPartnerDialog);
        connect(&middleDlg, &MiddleDialog::showTrumpDialog, this, &MainWindow::showTrumpDialog);
        connect(&middleDlg, &MiddleDialog::refreshCardWidgets, widget, &MainWidget::refreshCardWidgets);

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
        gamedata.playerArr[PLAYER_1].cardArr.sort();
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

void MainWindow::showBidResultMessage()
{
    std::string bidResultMsg = gamedata.playerArr[gamedata.roundInfo.bidPlayer].getPlayerName() + " won the bid for " +
                               std::to_string(gamedata.roundInfo.bidAmount) + ". " + "Bid updated.";

    MessageBox msgBox;
    msgBox.setText(QString::fromStdString(bidResultMsg));
    msgBox.setWindowTitle("Bid Result");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
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

void MainWindow::showPartnerMessage()
{
    std::string msg = gamedata.playerArr[gamedata.roundInfo.partnerPlayerNum].getPlayerName() + " is the partner. Teams updated.";

    MessageBox msgBox;
    msgBox.showCards({gamedata.roundInfo.partnerCard});
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setWindowTitle("Partner card");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showHandResultMessage(PlayerCardPair &winningPair)
{
    auto msg = [&winningPair]() -> std::string {
        return gamedata.playerArr[winningPair.playerNum].getPlayerName() +
               " won the hand for " + std::to_string(gamedata.handInfo.points) + " points with the";
    }();

    MessageBox msgBox;
    msgBox.showCards({winningPair.card});
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setWindowTitle("Hand result");
    msgBox.move({340, 250});
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showNestResultMessage(PlayerCardPair &winningPair)
{
    std::string msg = gamedata.playerArr[winningPair.playerNum].getPlayerName() +
                      " won the nest. Nest had " + std::to_string(gamedata.roundInfo.pointsMiddle) + " points.";

    MessageBox msgBox;
    msgBox.changeCardArrayDrawPosition(DRAW_POSITION_MESSAGE_BOX_NEST);
    msgBox.showCards(gamedata.nest);
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setWindowTitle("Last hand");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showWrongNumNestCardsMessage()
{
    MessageBox msgBox;
    msgBox.setText("Nest must have exactly 5 cards");
    msgBox.setWindowTitle("Nest problem");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showTooManyNestCardsMessage(int numMiddleCardsSelected, int numMiddleCardsAllowed)
{
    std::string msg = std::to_string(numMiddleCardsSelected) + " are selected but only " + std::to_string(numMiddleCardsAllowed) +
                      " are allowed to be selected.\n\nReview selected cards.\n\nTip: Use \"Highlight nest cards\" to see selected cards.";

    MessageBox msgBox;
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setWindowTitle("Nest problem");
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

void MainWindow::showInvalidCardPlayedMessage(ClickableCard *clickableCard)
{
    MessageBox msgBox;
    msgBox.showCards({clickableCard->data});
    msgBox.setText("Invalid card clicked. Must follow suit.");
    msgBox.setWindowTitle("Invalid card");
    Utils::Ui::moveParentlessDialog(&msgBox, this, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void MainWindow::showInvalidMiddleDialogMessage()
{
    MessageBox msgBox;
    msgBox.setText("Trump and partner card must be selected");
    msgBox.setWindowTitle("Invalid selection");
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
    for (auto playerNum : std::vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
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
    default:
        return "slot3.db";
    }
}