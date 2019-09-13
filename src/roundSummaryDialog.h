#ifndef ROUNDSUMMARYDIALOG_H
#define ROUNDSUMMARYDIALOG_H

#include "common.h"
#include "scaledWidgets.h"

// forward declarations
struct GameData;

// global declarations
extern GameData gamedata;

const QSize ROUND_SUMMARY_DIALOG_SIZE = {500, 250};

class RoundSummaryDialog : public QDialog
{
    QVBoxLayout *titleLayout;
    QLabel *summaryLabel;

    QVBoxLayout *namesLayout;
    QLabel *player1ScoreLabel;
    QLabel *player2ScoreLabel;
    QLabel *player3ScoreLabel;
    QLabel *player4ScoreLabel;

    QVBoxLayout *scoresLayout;
    QLabel *player1Score;
    QLabel *player2Score;
    QLabel *player3Score;
    QLabel *player4Score;
    
    QHBoxLayout *namesScoresLayout;

    QHBoxLayout *buttonLayout;
    QPushButton *okButton;
    
    QVBoxLayout *mainLayout; // titleLayout, namesScoresLayout, buttonLayout

public:
    RoundSummaryDialog(QWidget *parent = nullptr);
    virtual void rescale();

    void updateScores(const std::map<int, int> &roundScores);
};

#endif
