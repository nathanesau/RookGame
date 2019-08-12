#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <string>
#include <vector>

#include "bidDialog.h"
#include "cpuPlayer.h"
#include "gameController.h"
#include "messageBox.h"
#include "utils.h"

using namespace std;

BidDialog::BidDialog(QMainWindow *pMainWindow, QWidget *parent) : mainWindow(pMainWindow),
                                                                  ScaledQDialog(true, parent)
{
    ui.setupUi(this);

    setupComboBox(40, 120, 5);

    ui.player1Label->setText(QString::fromStdString(gc.data.playerArr[PLAYER_1].getPlayerName()));
    ui.player2Label->setText(QString::fromStdString(gc.data.playerArr[PLAYER_2].getPlayerName()));
    ui.player3Label->setText(QString::fromStdString(gc.data.playerArr[PLAYER_3].getPlayerName()));
    ui.player4Label->setText(QString::fromStdString(gc.data.playerArr[PLAYER_4].getPlayerName()));

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
    setWindowFlags(Qt::WindowStaysOnTopHint);
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
    gc.data.playerArr[PLAYER_1].bid = ui.bidAmountComboBox->currentText().toInt();
    gc.data.roundInfo.bidAmount = gc.data.playerArr[PLAYER_1].bid;
    gc.data.roundInfo.bidPlayer = PLAYER_1;

    gc.getCpuBids();

    auto showBid = [](ScaledQLabel *label, int bid, bool passed) {
        QString bidText = (passed) ? "Pass" : QString::number(bid);
        label->setText(bidText);
    };

    showBid(ui.player1BidLabel, gc.data.playerArr[PLAYER_1].bid, gc.data.playerArr[PLAYER_1].passed);
    showBid(ui.player2BidLabel, gc.data.playerArr[PLAYER_2].bid, gc.data.playerArr[PLAYER_2].passed);
    showBid(ui.player3BidLabel, gc.data.playerArr[PLAYER_3].bid, gc.data.playerArr[PLAYER_3].passed);
    showBid(ui.player4BidLabel, gc.data.playerArr[PLAYER_4].bid, gc.data.playerArr[PLAYER_4].passed);

    if (gc.getNumPassed() == 3) // bidding round over
    {
        QDialog::accept(); // close bid dialog
        showBidResultMsgBox();
    }
    else
    {
        setupComboBox(gc.data.roundInfo.bidAmount + 5, 120, 5);
    }
}

void BidDialog::onPassButtonPressed()
{
    gc.data.playerArr[PLAYER_1].passed = true;
    gc.data.roundInfo.bidAmount = 40; // bid cannot be less than 40

    while (gc.getNumPassed() != 3)
    {
        gc.getCpuBids();
    }

    auto newNest = cpu.getChosenNest(gc.data.roundInfo.bidPlayer);

    auto &cardArr = gc.data.playerArr[gc.data.roundInfo.bidPlayer].cardArr;
    cardArr.append({&gc.data.nest});
    cardArr.remove(newNest);

    gc.data.roundInfo.trump = cpu.getChosenTrump(gc.data.roundInfo.bidPlayer);
    gc.data.roundInfo.partnerCard = cpu.getChosenPartner(gc.data.roundInfo.bidPlayer);

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
    string bidResultMsg = gc.data.playerArr[gc.data.roundInfo.bidPlayer].getPlayerName() + " won the bid for " +
                          to_string(gc.data.roundInfo.bidAmount) + ". " + "Bid updated.";

    MessageBox msgBox;
    msgBox.setText(QString::fromStdString(bidResultMsg));
    msgBox.setWindowTitle("Bid Result");
    Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
    msgBox.exec();
}