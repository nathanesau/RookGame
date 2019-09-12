#ifndef ROUNDSUMMARYDIALOG_H
#define ROUNDSUMMARYDIALOG_H

#include "common.h"
#include "scaledWidgets.h"

// forward declarations
struct GameData;

// global declarations
extern GameData gamedata;

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

    void updateScores(const std::map<int, int> &roundScores);
};

#endif