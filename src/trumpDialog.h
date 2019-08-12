#ifndef TRUMPDIALOG_H
#define TRUMPDIALOG_H

#include <string>

#include "scaledWidgets.h"

using namespace std;

// forward declarations
class TrumpDialog;

class TrumpDialogLabel : public ScaledQLabel
{
    Q_OBJECT

public:
    TrumpDialogLabel(TrumpDialog *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);
};

const QSize TRUMP_DIALOG_SIZE = {600, 250};

// for selecting a trump suit
class TrumpDialog : public ScaledQDialog
{
    int &suitSelected; // output of dialog

    TrumpDialogLabel *blackLabel;
    TrumpDialogLabel *greenLabel;
    TrumpDialogLabel *redLabel;
    TrumpDialogLabel *yellowLabel;

public:
    TrumpDialog(int &pSuitSelected, QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    void onTrumpLabelClicked(TrumpDialogLabel *label);
};

#endif