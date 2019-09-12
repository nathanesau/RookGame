#ifndef BIDDIALOG_H
#define BIDDIALOG_H

#include "common.h"
#include "scaledWidgets.h"

// forward declarations
class CpuDecisionMaker;
struct GameData;

// global declarations
extern CpuDecisionMaker cpu;
extern GameData gamedata;

const QSize BID_DIALOG_SIZE = {661, 302};

class BidDialog : public ScaledQDialog
{
    ScaledQLabel *player1Label;
    ScaledQLabel *player2Label;
    ScaledQLabel *player3Label;
    ScaledQLabel *player4Label;
    ScaledQLabel *player1BidLabel;
    ScaledQLabel *player2BidLabel;
    ScaledQLabel *player3BidLabel;
    ScaledQLabel *player4BidLabel;
    ScaledQPushButton *bidButton;
    ScaledQPushButton *passButton;
    ScaledQComboBox *bidAmountComboBox;
    ScaledQLabel *bidAmountLabel;

    void setupComboBox(int minBid, int maxBid, int incr);
    void setupUi();

public:
    BidDialog(QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    void onBidButtonPressed();
    void onPassButtonPressed();

private:
    void showBidResultMsgBox();
    void getCpuBids();
    int getNumPassed();
};

#endif
