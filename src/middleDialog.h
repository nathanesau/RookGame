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
    Q_OBJECT

    CardVector originalNest;

private:
    Ui::MiddleDialog ui;
    
    ClickableCardArray *nestCards;
    ClickableCardArray *partnerCards;

signals:
    void showInvalidMiddleDialogMessage();
    void showNestDialog(const CardVector &originalNest);
    void showPartnerDialog();
    void showTrumpDialog();
    void refreshCardWidgets(GameData &pData);

public:
    MiddleDialog(QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    virtual void onCardClicked(ClickableCard *clickableCard);

private:
    void onSelectNestButtonPressed();
    void onAutoSelectNestButtonPressed();
    void onSelectTrumpButtonPressed();
    void onAutoSelectTrumpButtonPressed();
    void onSelectPartnerButtonPressed();
    void onAutoSelectPartnerButtonPressed();
    void onOkButtonPressed();

    void setupTrumpLabel(int suit);
};

#endif
