#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <string>
#include <vector>

#include "bidDialog.h"
#include "cpuPlayer.h"
#include "gameData.h"
#include "messageBox.h"
#include "utils.h"

using namespace std;

BidDialog::BidDialog(QMainWindow *pMainWindow, QWidget *parent) : mainWindow(pMainWindow),
                                                                  ScaledQDialog(true, parent)
{
    ui.setupUi(this);

    setupComboBox(40, 120, 5);

    ui.player1Label->setText(QString::fromStdString(gamedata.playerArr[PLAYER_1].getPlayerName()));
    ui.player2Label->setText(QString::fromStdString(gamedata.playerArr[PLAYER_2].getPlayerName()));
    ui.player3Label->setText(QString::fromStdString(gamedata.playerArr[PLAYER_3].getPlayerName()));
    ui.player4Label->setText(QString::fromStdString(gamedata.playerArr[PLAYER_4].getPlayerName()));

    ui.bidAmountLabel->adjustSize();
    ui.player1Label->adjustSize();
    ui.player2Label->adjustSize();
    ui.player3Label->adjustSize();
    ui.player4Label->adjustSize();

    QObject::connect(ui.bidButton, &QPushButton::pressed, this, &BidDialog::onBidButtonPressed);
    QObject::connect(ui.passButton, &QPushButton::pressed, this, &BidDialog::onPassButtonPressed);

    resize(BID_DIALOG_SIZE);
    setWindowIcon(QIcon(":rookicon.gif"));
    setStyleSheet("background-color: white");
    #ifdef WINDOW_ALWAYS_ON_TOP
    setWindowFlags(Qt::WindowStaysOnTopHint);
    #endif
}

void BidDialog::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto label : vector<ScaledQLabel *>{ui.player1Label, ui.player2Label, ui.player3Label, ui.player4Label,
                                             ui.player1BidLabel, ui.player2BidLabel, ui.player3BidLabel, ui.player4BidLabel,
                                             ui.bidAmountLabel})
        label->rescale();

    for (auto button : vector<ScaledQPushButton *>{ui.bidButton, ui.passButton})
        button->rescale();

    for (auto comboBox : vector<ScaledQComboBox *>{ui.bidAmountComboBox})
        comboBox->rescale();
}

void BidDialog::reject()
{
    // do nothing
}

void BidDialog::onBidButtonPressed()
{
    gamedata.playerArr[PLAYER_1].bid = ui.bidAmountComboBox->currentText().toInt();
    gamedata.roundInfo.bidAmount = gamedata.playerArr[PLAYER_1].bid;
    gamedata.roundInfo.bidPlayer = PLAYER_1;

    getCpuBids();

    auto showBid = [](ScaledQLabel *label, int bid, bool passed) {
        QString bidText = (passed) ? "Pass" : QString::number(bid);
        label->setText(bidText);
    };

    showBid(ui.player1BidLabel, gamedata.playerArr[PLAYER_1].bid, gamedata.playerArr[PLAYER_1].passed);
    showBid(ui.player2BidLabel, gamedata.playerArr[PLAYER_2].bid, gamedata.playerArr[PLAYER_2].passed);
    showBid(ui.player3BidLabel, gamedata.playerArr[PLAYER_3].bid, gamedata.playerArr[PLAYER_3].passed);
    showBid(ui.player4BidLabel, gamedata.playerArr[PLAYER_4].bid, gamedata.playerArr[PLAYER_4].passed);

    if (getNumPassed() == 3) // bidding round over
    {
        QDialog::accept(); // close bid dialog
        showBidResultMsgBox();
    }
    else
    {
        setupComboBox(gamedata.roundInfo.bidAmount + 5, 120, 5);
    }
}

void BidDialog::onPassButtonPressed()
{
    gamedata.playerArr[PLAYER_1].passed = true;
    gamedata.roundInfo.bidAmount = max(gamedata.roundInfo.bidAmount, 40); // bid cannot be less than 40

    while (getNumPassed() != 3)
    {
        getCpuBids();
    }

    auto newNest = cpu.getChosenNest(gamedata.roundInfo.bidPlayer);

    auto &cardArr = gamedata.playerArr[gamedata.roundInfo.bidPlayer].cardArr;
    cardArr.insert(cardArr.end(), gamedata.nest.begin(), gamedata.nest.end());
    cardArr.remove(newNest);

    gamedata.roundInfo.trump = cpu.getChosenTrump(gamedata.roundInfo.bidPlayer);
    gamedata.roundInfo.partnerCard = cpu.getChosenPartner(gamedata.roundInfo.bidPlayer);

    QDialog::reject(); // close bid dialog
    showBidResultMsgBox();
}

void BidDialog::setupComboBox(int minBid, int maxBid, int incr)
{
    ui.bidAmountComboBox->clear();

    for (int bid = minBid; bid <= maxBid; bid += incr)
    {
        QString itemText = QString::number(bid);
        ui.bidAmountComboBox->addItem(itemText);
    }

    ui.bidAmountComboBox->showNormal();
}

void BidDialog::showBidResultMsgBox()
{
    string bidResultMsg = gamedata.playerArr[gamedata.roundInfo.bidPlayer].getPlayerName() + " won the bid for " +
                          to_string(gamedata.roundInfo.bidAmount) + ". " + "Bid updated.";

    MessageBox msgBox;
    msgBox.setText(QString::fromStdString(bidResultMsg));
    msgBox.setWindowTitle("Bid Result");
    Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
    msgBox.exec();
}

void BidDialog::getCpuBids()
{
    for (auto playerNum : vector<int>{PLAYER_2, PLAYER_3, PLAYER_4})
    {
        auto &player = gamedata.playerArr[playerNum];

        if (player.passed)
            continue;

        if (player.bid == gamedata.roundInfo.bidAmount)
            continue;

        int prevBid = player.bid;
        player.bid = cpu.getBid(playerNum);
        player.passed = (player.bid == prevBid);

        if (player.bid > gamedata.roundInfo.bidAmount)
        {
            gamedata.roundInfo.bidAmount = gamedata.playerArr[playerNum].bid;
            gamedata.roundInfo.bidPlayer = playerNum;
        }
    }
}

int BidDialog::getNumPassed()
{
    // get number of players who passed bid
    return accumulate(gamedata.playerArr.begin(), gamedata.playerArr.end(), 0, [&](int a, Player &b) { return a + (b.passed); });
}