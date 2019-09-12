#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "mainWidget.h"
#include "scaledWidgets.h"

// forward declarations
struct GameData;
class GameDatabase;

// global declarations
extern GameData gamedata;
extern GameDatabase db;

const QSize MAIN_WINDOW_SIZE = {1200, 850};

class MainWindow : public ScaledQMainWindow
{
    Q_OBJECT

    MainWidget *widget; // central widget

    ScaledQProgressBar *progressBar; // display progress of time consuming actions

    QMenu *fileMenu;
    QAction* newGameAction;
    QAction* saveGameAction;
    QAction* loadGameAction;
    QAction *exitMainMenuAction;
    QAction* closeAction;

    QMenu *editMenu;
    QAction *preferencesAction;

    QMenu *gameMenu;
    QAction* viewScoresAction;

    QMenu *helpMenu;
    QAction *aboutAction;

    void setupActions();
    void setupMenus();

public:
    MainWindow(QWidget *parent = nullptr);
    virtual void rescale();

    // preferences dialog - appearance page
    void refreshNameTags();

    // slots
    void onNewGameAction();
    void onLoadGameAction();
    void onSaveGameAction();
    void onPreferencesAction();
    void onExitMainMenuAction();
    void onCloseAction();
    void onViewScoresAction();
    void onCheckUpdatesAction();
    void onAboutAction();
    void startNewRound();

    // messages
    void showAboutMessage();
    void showBidResultMessage();
    void showExitMainMenuMessage();
    void showGameStartingMessage();
    void showInvalidCardPlayedMessage(ClickableCard *clickableCard);
    void showInvalidMiddleDialogMessage();
    void showLoadGameMessage();
    void showNewGameMessage();
    void showNewRoundMessage();
    void showSaveGameMessage();
    void showPartnerMessage();
    void showHandResultMessage(PlayerCardPair &winningPair);
    void showNestResultMessage(PlayerCardPair &winningPair);
    void showWrongNumNestCardsMessage();
    void showTooManyNestCardsMessage(int numMiddleCardsSelected, int numMiddleCardsAllowed);

    // dialogs
    void showNestDialog(const CardVector &originalNest);
    void showRoundSummaryDialog();
    void showPartnerDialog();
    void showTrumpDialog();

private:
    bool hasRoundStarted() const;

    void setRookSuitToTrump();
    QString getSlotDbName(int slotNumber) const;
};

#endif