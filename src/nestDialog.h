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
class NestDialog : public QDialog
{
    Q_OBJECT

    // output of dialog is gamedata (modify directly)

    CardVector originalNest;
    CardStyleMap originalNestStyles;

private:
    QVBoxLayout *middleCardsLayout;
    QLabel *nestCardsLabel;
    LayoutClickableCardArray *nestCards;

    QVBoxLayout *newHandLayout;
    QLabel *player1CardsPreviewLabel;
    LayoutClickableCardArray *player1CardsPreview;

    QHBoxLayout *buttonsCheckBoxLayout;
    QPushButton *autoChooseNestButton;
    QPushButton *resetNestButton;
    QPushButton *doneNestButton;
    QCheckBox *highlightCardsCheckBox;

    QVBoxLayout *mainLayout;

signals:
    void showWrongNumNestCardsMessage();
    void showTooManyNestCardsMessage(int numMiddleCardsSelected, int numMiddleCardsAllowed);

private:
    void initializeMiddleCardsLayout();
    void initializeNewHandLayout();
    void initializeButtonsCheckBoxLayout();

public:
    NestDialog(CardVector pOriginalNest, QWidget *parent = nullptr);

    void setOriginalNestStyles(std::string style);

    virtual void onCardClicked(ClickableCard *clickableCard);

    void autoChooseNestButtonPressed();
    void resetNestButtonPressed();
    void doneNestButtonPressed();
    void highlightCardsCheckBoxPressed();
};

#endif