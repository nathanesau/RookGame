#ifndef TRUMPDIALOG_H
#define TRUMPDIALOG_H

#include "common.h"
#include "commonWidgets.h"
#include "scaledWidgets.h"

// forward declarations
class TrumpDialog;

const QSize TRUMP_DIALOG_SIZE = {250, 300};

// for selecting a trump suit
class TrumpDialog : public ScaledQDialog
{
    int &suitSelected; // output of dialog

    QVBoxLayout *mainLayout;
    ClickableLabel *blackLabel;
    ClickableLabel *greenLabel;
    ClickableLabel *redLabel;
    ClickableLabel *yellowLabel;

public:
    TrumpDialog(int &pSuitSelected, QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    void onTrumpLabelClicked(ClickableLabel *label);
};

#endif