#ifndef GAMEMENUWIDGET_H
#define GAMEMENUWIDGET_H

#include "scaledWidgets.h"

// forward declarations
class MainWindow;

const QSize GAME_MENU_WIDGET_SIZE = {400, 300};

class GameMenuWidget : public ScaledQDialog
{
    MainWindow *mainWindow; // non-owning

    ScaledQLabel *menuTitleLabel;
    ScaledQPushButton *newGameButton;
    ScaledQPushButton *newRoundButton;
    ScaledQPushButton *saveGameButton;
    ScaledQPushButton *loadGameButton;
    ScaledQPushButton *quitGameButton;

public:
    GameMenuWidget(MainWindow *pMainWindow, QWidget *parent = nullptr);
    void rescale();

    void onNewGameButtonPressed();
    void onNewRoundButtonPressed();
    void onSaveGameButtonPressed();
    void onLoadGameButtonPressed();
    void onQuitGameButtonPressed();
};

#endif