#ifndef NESTDIALOG_H
#define NESTDIALOG_H

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <string>

#include "card.h"
#include "clickableCard.h"
#include "common.h"
#include "scaledWidgets.h"

using namespace std;

// forward declarations
class GameController;

// global declarations
extern GameController gc;

const QSize NEST_DIALOG_SIZE = {911, 506};

// for selecting cards from nest
class NestDialog : public QDialogWithClickableCardArray
{
    // output of dialog is gc.data (modify directly)

    CardVector originalNest;
    CardStyleMap originalNestStyles;

private:
    QMainWindow *mainWindow; // non-owning

    ScaledQPushButton *autoChooseNestButton;
    ScaledQPushButton *resetNestButton;
    ScaledQPushButton *doneNestButton;

    ScaledQLabel *centerCardsLabel;
    ClickableCardArray *centerCards;

    ScaledQLabel *bottomCardsPreviewLabel;
    ClickableCardArray *bottomCardsPreview;

    ScaledQCheckBox *highlightCardsCheckBox;

public:
    NestDialog(CardVector pOriginalNest, QMainWindow *pMainWindow, QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    void setOriginalNestStyles(string style);

    virtual void onCardClicked(ClickableCard *clickableCard);
    virtual void onCardHoverEnter(ClickableCard *clickableCard);
    virtual void onCardHoverLeave(ClickableCard *clickableCard);

    static void autoChooseNest();

    void autoChooseNestButtonPressed();
    void resetNestButtonPressed();
    void doneNestButtonPressed();
    void highlightCardsCheckBoxPressed();
};

#endif