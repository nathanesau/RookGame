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
    void refreshNameTags(bool showNameTags);

    void onNewGameAction();
    void onLoadGameAction();
    void onSaveGameAction();
    void onPreferencesAction();
    void onExitMainMenuAction();
    void onCloseAction();
    void onViewScoresAction();
    void onCheckUpdatesAction();
    void onAboutAction();

    // main function for game
    void startNewRound();

    // messages
    void showNewGameMessage();
    void showSaveGameMessage();
    void showLoadGameMessage();
    void showExitMainMenuMessage();
    void showNewRoundMessage();
    void showGameStartingMessage();
    void showAboutMessage();

private:
    bool hasRoundStarted() const;

    void setRookSuitToTrump();
    QString getSlotDbName(int slotNumber) const;
};

#endif