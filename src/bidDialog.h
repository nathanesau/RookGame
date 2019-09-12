#ifndef BIDDIALOG_H
#define BIDDIALOG_H

#include "common.h"
#include "scaledWidgets.h"
#include "ui_bidDialog.h" // uic -o ui_bidDialog.h bidDialog.ui

// forward declarations
class CpuDecisionMaker;
struct GameData;

// global declarations
extern CpuDecisionMaker cpu;
extern GameData gamedata;

const QSize BID_DIALOG_SIZE = {661, 302};

class BidDialog : public ScaledQDialog
{
    QMainWindow *mainWindow; // non-owning

    Ui::BidDialog ui;

    void setupComboBox(int minBid, int maxBid, int incr);

public:
    BidDialog(QMainWindow *pMainWindow, QWidget *parent = nullptr);
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
