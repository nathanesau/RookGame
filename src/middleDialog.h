#ifndef MIDDLEDIALOG_H
#define MIDDLEDIALOG_H

#include "card.h"
#include "clickableCard.h"
#include "common.h"
#include "scaledWidgets.h"
#include "ui_middleDialog.h" // uic -o ui_middleDialog.h middleDialog.ui

// forward declarations
struct GameData;

// global declarations
extern GameData gamedata;

const QSize MIDDLE_DIALOG_SIZE = {724, 435};

// output: gamedata.nest, gamedata.roundInfo.trump, gamedata.roundInfo.partnerCard
class MiddleDialog : public QDialogWithClickableCardArray
{
    CardVector originalNest;

private:
    MainWidget *mainWidget; // non-owning
    QMainWindow *mainWindow; // non-owning

    Ui::MiddleDialog ui;
    
    ClickableCardArray *nestCards;
    ClickableCardArray *partnerCards;

public:
    MiddleDialog(MainWidget *pMainWidget, QMainWindow *pMainWindow, QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    virtual void onCardClicked(ClickableCard *clickableCard);
    virtual void onCardHoverEnter(ClickableCard *clickableCard);
    virtual void onCardHoverLeave(ClickableCard *clickableCard);

private:
    void selectNestButtonPressed();
    void autoSelectNestButtonPressed();
    void selectTrumpButtonPressed();
    void autoSelectTrumpButtonPressed();
    void selectPartnerButtonPressed();
    void autoSelectPartnerButtonPressed();
    void okButtonPressed();

    void setupTrumpLabel(int suit);
};

#endif
