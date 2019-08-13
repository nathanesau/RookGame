#ifndef BIDDIALOG_H
#define BIDDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QMainWindow>

#include "common.h"
#include "scaledWidgets.h"
#include "ui_BidDialog.h" // uic -o ui_bidDialog.h bidDialog.ui

// forward declarations
class CpuPlayer;
struct GameData;

// global declarations
extern CpuPlayer cpu;
extern GameData gamedata;

const QSize BID_DIALOG_SIZE = {661, 302};

class BidDialog : public ScaledQDialog
{
    QMainWindow *mainWindow; // non-owning

    Ui::BidDialog ui;

public:
    BidDialog(QMainWindow *pMainWindow, QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    void onBidButtonPressed();
    void onPassButtonPressed();

    void setupComboBox(int minBid, int maxBid, int incr);

private:
    void showBidResultMsgBox();
    void getCpuBids();
    int getNumPassed();
};

#endif