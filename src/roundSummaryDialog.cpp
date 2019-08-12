#include <string>
#include <QPoint>
#include <QString>

#include "gameController.h"
#include "roundSummaryDialog.h"
#include "player.h"

using namespace std;

RoundSummaryDialog::RoundSummaryDialog(QWidget *parent) : ScaledQDialog(true, parent)
{
    auto setupLabel = [this](ScaledQLabel *label, QString text, QSize size, QPoint pos) {
        label->setParent(this);
        label->setText(text);
        label->setFont(QFont("Times", 10));
        label->resize(size.width(), size.height());
        label->move(pos);
    };

    summaryLabel = new ScaledQLabel;
    summaryLabel->setParent(this);
    summaryLabel->setText("Scores for this round...");
    summaryLabel->setFont(QFont("Times", 12, QFont::Weight::Bold));
    summaryLabel->resize(250, 25);
    summaryLabel->move({25, 25});

    player1ScoreLabel = new ScaledQLabel;
    setupLabel(player1ScoreLabel, QString::fromStdString(gc.data.playerArr[PLAYER_1].getPlayerName() + ": "), {100, 25}, {25, 75});

    player1Score = new ScaledQLabel;
    setupLabel(player1Score, "", {100, 25}, {125, 75});

    player2ScoreLabel = new ScaledQLabel;
    setupLabel(player2ScoreLabel, QString::fromStdString(gc.data.playerArr[PLAYER_2].getPlayerName() + ": "), {100, 25}, {25, 100});

    player2Score = new ScaledQLabel;
    setupLabel(player2Score, "", {100, 25}, {125, 100});

    player3ScoreLabel = new ScaledQLabel;
    setupLabel(player3ScoreLabel, QString::fromStdString(gc.data.playerArr[PLAYER_3].getPlayerName() + ": "), {100, 25}, {25, 125});

    player3Score = new ScaledQLabel;
    setupLabel(player3Score, "", {100, 25}, {125, 125});

    player4ScoreLabel = new ScaledQLabel;
    setupLabel(player4ScoreLabel, QString::fromStdString(gc.data.playerArr[PLAYER_4].getPlayerName() + ": "), {100, 25}, {25, 150});

    player4Score = new ScaledQLabel;
    setupLabel(player4Score, "", {100, 25}, {125, 150});

    okButton = new ScaledQPushButton;
    okButton->setParent(this);
    okButton->setText("OK");
    okButton->setFont(QFont("Times", 10));
    okButton->resize(50, 25);
    okButton->move({400, 200});

    QObject::connect(okButton, &QPushButton::pressed, this, &RoundSummaryDialog::accept);

    resize(ROUND_SUMMARY_DIALOG_SIZE);
    setWindowTitle("Round summary");
    setWindowIcon(QIcon(":rookicon.gif"));
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setStyleSheet("background-color: white");
}

void RoundSummaryDialog::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto label : vector<ScaledQLabel *>{summaryLabel, player1ScoreLabel, player1Score, player2ScoreLabel,
                                             player2Score, player3ScoreLabel, player3Score, player4ScoreLabel,
                                             player4Score})
        label->rescale();

    for (auto button : vector<ScaledQPushButton *>{okButton})
        button->rescale();
}

void RoundSummaryDialog::updateScores(map<int, int> roundScores)
{
    auto updateLabel = [this](ScaledQLabel *label, int score) {
        string prefix = "";
        if (score > 0)
            prefix = "+";
        else if (score < 0)
            prefix = "-";

        string text = prefix + to_string(score);
        label->setText(QString::fromStdString(text));
    };

    updateLabel(player1Score, roundScores[PLAYER_1]);
    updateLabel(player2Score, roundScores[PLAYER_2]);
    updateLabel(player3Score, roundScores[PLAYER_3]);
    updateLabel(player4Score, roundScores[PLAYER_4]);
}