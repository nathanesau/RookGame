#ifndef GAMEMENUWIDGET_H
#define GAMEMENUWIDGET_H

#include "common.h"
#include "scaledWidgets.h"

const QSize GAME_MENU_WIDGET_SIZE = {400, 300};

class GameMenuWidget : public ScaledQDialog
{
    Q_OBJECT

    ScaledQLabel *menuTitleLabel;
    ScaledQPushButton *newGameButton;
    ScaledQPushButton *newRoundButton;
    ScaledQPushButton *saveGameButton;
    ScaledQPushButton *loadGameButton;
    ScaledQPushButton *quitGameButton;

signals:
    void newGameButtonPressed();
    void newRoundButtonPressed();
    void saveGameButtonPressed();
    void loadGameButtonPressed();
    void quitGameButtonPressed();

public:
    GameMenuWidget(QWidget *parent = nullptr);
    void rescale();
};

#endif