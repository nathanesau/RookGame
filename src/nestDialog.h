#ifndef NESTDIALOG_H
#define NESTDIALOG_H

#include "card.h"
#include "clickableCard.h"
#include "common.h"
#include "scaledWidgets.h"

// forward declarations
struct GameData;

// global declarations
extern GameData gamedata;

const QSize NEST_DIALOG_SIZE = {911, 506};

// for selecting cards from nest
class NestDialog : public QDialogWithClickableCardArray
{
    Q_OBJECT

    // output of dialog is gamedata (modify directly)

    CardVector originalNest;
    CardStyleMap originalNestStyles;

private:
    ScaledQPushButton *autoChooseNestButton;
    ScaledQPushButton *resetNestButton;
    ScaledQPushButton *doneNestButton;

    ScaledQLabel *nestCardsLabel;
    ClickableCardArray *nestCards;

    ScaledQLabel *player1CardsPreviewLabel;
    ClickableCardArray *player1CardsPreview;

    ScaledQCheckBox *highlightCardsCheckBox;

signals:
    void showWrongNumNestCardsMessage();
    void showTooManyNestCardsMessage(int numMiddleCardsSelected, int numMiddleCardsAllowed);

public:
    NestDialog(CardVector pOriginalNest, QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    void setOriginalNestStyles(std::string style);

    virtual void onCardClicked(ClickableCard *clickableCard);

    void autoChooseNestButtonPressed();
    void resetNestButtonPressed();
    void doneNestButtonPressed();
    void highlightCardsCheckBoxPressed();
};

#endif