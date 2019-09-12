#ifndef PARTNERDIALOG_H
#define PARTNERDIALOG_H

#include "clickableCard.h"
#include "common.h"
#include "scaledWidgets.h"

// forward declarations
struct GameData;
class PartnerDialog;

// global declarations
extern GameData gamedata;

class PartnerDialogLabel : public ScaledQLabel
{
    Q_OBJECT

public:
    PartnerDialogLabel(PartnerDialog *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);
};

const QSize PARTNER_DIALOG_SIZE = {850, 300};

// for selecting a partner card from available cards
class PartnerDialog : public QDialogWithClickableCardArray
{
    Card &cardSelected; // output of dialog

    CardVector blackCards;
    CardVector greenCards;
    CardVector redCards;
    CardVector yellowCards;
    CardVector wildCards;

private:
    PartnerDialogLabel *blackLabel;
    PartnerDialogLabel *greenLabel;
    PartnerDialogLabel *redLabel;
    PartnerDialogLabel *yellowLabel;
    PartnerDialogLabel *wildLabel;

    ScaledQPushButton *cancelButton;

    ClickableCardArray *partnerOptionsCards;

public:
    PartnerDialog(Card &pCardSelected, QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    void onPartnerLabelClicked(PartnerDialogLabel *label);

    virtual void onCardClicked(ClickableCard *clickableCard);

    void setupCardArrays();
};

#endif