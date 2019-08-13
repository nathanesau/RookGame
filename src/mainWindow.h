#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QTableWidget>
#include <string>

#include "common.h"
#include "mainWidget.h"
#include "scaledWidgets.h"

#include <memory>

using namespace std;

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
    QAction* quitAction;

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
    void onQuitAction();
    void onViewScoresAction();
    void onCheckUpdatesAction();
    void onAboutAction();

    // main function for game
    void startNewRound();

    void showNewRoundMessage();
    void showGameStartingMessage();

private:
    void setRookSuitToTrump();
    QString getSlotDbName(int slotNumber) const;
};

#endif