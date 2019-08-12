#ifndef ROUNDSUMMARYDIALOG_H
#define ROUNDSUMMARYDIALOG_H

#include<map>

#include "common.h"
#include "scaledWidgets.h"

using namespace std;

// forward declarations
class GameController;

// global declarations
extern GameController gc;

const QSize ROUND_SUMMARY_DIALOG_SIZE = {500, 250};

class RoundSummaryDialog : public ScaledQDialog
{
    ScaledQLabel *summaryLabel;

    ScaledQLabel *player1ScoreLabel;
    ScaledQLabel *player1Score;

    ScaledQLabel *player2ScoreLabel;
    ScaledQLabel *player2Score;

    ScaledQLabel *player3ScoreLabel;
    ScaledQLabel *player3Score;

    ScaledQLabel *player4ScoreLabel;
    ScaledQLabel *player4Score;

    ScaledQPushButton *okButton;

public:
    RoundSummaryDialog(QWidget *parent = nullptr);
    virtual void rescale();

    void updateScores(map<int, int> roundScores);
};

#endif