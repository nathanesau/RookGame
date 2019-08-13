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
class GameController;
class GameDatabase;

// global declarations
extern GameController gc;
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
    void updateNameTags(bool showNameTags);

    void onNewGameAction();
    void onLoadGameAction();
    void onSaveGameAction();
    void onPreferencesAction();
    void onQuitAction();
    void onViewScoresAction();
    void onCheckUpdatesAction();
    void onAboutAction();

    void startNewRound();
    void showNewRoundMessage();

private:
    QString getSlotDbName(int slotNumber) const;
};

#endif