#include "bidDialog.h"
#include "cpu.h"
#include "gameData.h"
#include "utils.h"

void BidDialog::setupUi()
{
    QFont labelFont;
    labelFont.setPointSize(12);

    QFont buttonFont;
    buttonFont.setPointSize(10);

    QFont titleFont;
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    titleFont.setWeight(75);

    player1Label = new ScaledQLabel;
    player1Label->setParent(this);
    player1Label->setGeometry(QRect(320, 230, 81, 21));
    player1Label->setFont(labelFont);
    player2Label = new ScaledQLabel;
    player2Label->setParent(this);
    player2Label->setGeometry(QRect(70, 110, 81, 21));
    player2Label->setFont(labelFont);
    player3Label = new ScaledQLabel;
    player3Label->setParent(this);
    player3Label->setGeometry(QRect(310, 20, 81, 21));
    player3Label->setFont(labelFont);
    player4Label = new ScaledQLabel;
    player4Label->setParent(this);
    player4Label->setGeometry(QRect(540, 110, 71, 21));
    player4Label->setFont(labelFont);

    player1BidLabel = new ScaledQLabel;
    player1BidLabel->setParent(this);
    player1BidLabel->setGeometry(QRect(340, 260, 41, 20));
    player1BidLabel->setFont(labelFont);
    player2BidLabel = new ScaledQLabel;
    player2BidLabel->setParent(this);
    player2BidLabel->setGeometry(QRect(90, 140, 41, 16));
    player2BidLabel->setFont(labelFont);
    player3BidLabel = new ScaledQLabel;
    player3BidLabel->setParent(this);
    player3BidLabel->setGeometry(QRect(330, 50, 41, 20));
    player3BidLabel->setFont(labelFont);
    player4BidLabel = new ScaledQLabel;
    player4BidLabel->setParent(this);
    player4BidLabel->setGeometry(QRect(560, 140, 41, 20));
    player4BidLabel->setFont(labelFont);

    bidButton = new ScaledQPushButton;
    bidButton->setText("Bid");
    bidButton->setParent(this);
    bidButton->setGeometry(QRect(160, 200, 71, 31));
    bidButton->setFont(buttonFont);

    passButton = new ScaledQPushButton;
    passButton->setText("Pass");
    passButton->setParent(this);
    passButton->setGeometry(QRect(440, 200, 81, 31));
    passButton->setFont(buttonFont);

    bidAmountComboBox = new ScaledQComboBox;
    bidAmountComboBox->setParent(this);
    bidAmountComboBox->setGeometry(QRect(280, 150, 141, 21));
    bidAmountComboBox->setFont(labelFont);

    bidAmountLabel = new ScaledQLabel;
    bidAmountLabel->setText("Bid Amount");
    bidAmountLabel->setParent(this);
    bidAmountLabel->setGeometry(QRect(300, 110, 121, 21));
    bidAmountLabel->setFont(titleFont);
}

BidDialog::BidDialog(QWidget *parent) : ScaledQDialog(true, parent)
{
    setupUi();

    setupComboBox(40, 120, 5);

    player1Label->setText(QString::fromStdString(gamedata.playerArr[PLAYER_1].getPlayerName()));
    player2Label->setText(QString::fromStdString(gamedata.playerArr[PLAYER_2].getPlayerName()));
    player3Label->setText(QString::fromStdString(gamedata.playerArr[PLAYER_3].getPlayerName()));
    player4Label->setText(QString::fromStdString(gamedata.playerArr[PLAYER_4].getPlayerName()));

    player1BidLabel->setText("0");
    player2BidLabel->setText("0");
    player3BidLabel->setText("0");
    player4BidLabel->setText("0");

    bidAmountLabel->adjustSize();
    player1Label->adjustSize();
    player2Label->adjustSize();
    player3Label->adjustSize();
    player4Label->adjustSize();

    connect(bidButton, &QPushButton::pressed, this, &BidDialog::onBidButtonPressed);
    connect(passButton, &QPushButton::pressed, this, &BidDialog::onPassButtonPressed);

    resize(BID_DIALOG_SIZE);
    setWindowIcon(QIcon(":rookicon.gif"));
}

void BidDialog::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto label : std::vector<ScaledQLabel *>{player1Label, player2Label, player3Label, player4Label,
                                                  player1BidLabel, player2BidLabel, player3BidLabel, player4BidLabel,
                                                  bidAmountLabel})
        label->rescale();

    for (auto button : std::vector<ScaledQPushButton *>{bidButton, passButton})
        button->rescale();

    for (auto comboBox : std::vector<ScaledQComboBox *>{bidAmountComboBox})
        comboBox->rescale();
}

void BidDialog::reject()
{
    // do nothing
}

void BidDialog::onBidButtonPressed()
{
    gamedata.playerArr[PLAYER_1].bid = bidAmountComboBox->currentText().toInt();
    gamedata.roundInfo.bidAmount = gamedata.playerArr[PLAYER_1].bid;
    gamedata.roundInfo.bidPlayer = PLAYER_1;

    getCpuBids();

    auto showBid = [](ScaledQLabel *label, int bid, bool passed) {
        QString bidText = (passed) ? "Pass" : QString::number(bid);
        label->setText(bidText);
    };

    showBid(player1BidLabel, gamedata.playerArr[PLAYER_1].bid, gamedata.playerArr[PLAYER_1].passed);
    showBid(player2BidLabel, gamedata.playerArr[PLAYER_2].bid, gamedata.playerArr[PLAYER_2].passed);
    showBid(player3BidLabel, gamedata.playerArr[PLAYER_3].bid, gamedata.playerArr[PLAYER_3].passed);
    showBid(player4BidLabel, gamedata.playerArr[PLAYER_4].bid, gamedata.playerArr[PLAYER_4].passed);

    if (getNumPassed() == 3) // bidding round over
    {
        QDialog::accept(); // close bid dialog
    }
    else
    {
        setupComboBox(gamedata.roundInfo.bidAmount + 5, 120, 5);
    }
}

void BidDialog::onPassButtonPressed()
{
    gamedata.playerArr[PLAYER_1].passed = true;
    gamedata.roundInfo.bidAmount = std::max(gamedata.roundInfo.bidAmount, 40); // bid cannot be less than 40

    while (getNumPassed() != 3)
    {
        getCpuBids();
    }

    auto &bidPlayer = gamedata.playerArr[gamedata.roundInfo.bidPlayer];

    bidPlayer.cpu->selectNest();
    bidPlayer.cpu->selectTrump();
    bidPlayer.cpu->selectPartner();

    QDialog::reject(); // close bid dialog
}

void BidDialog::setupComboBox(int minBid, int maxBid, int incr)
{
    bidAmountComboBox->clear();

    for (int bid = minBid; bid <= maxBid; bid += incr)
    {
        QString itemText = QString::number(bid);
        bidAmountComboBox->addItem(itemText);
    }

    bidAmountComboBox->showNormal();
}

void BidDialog::getCpuBids()
{
    for (auto playerNum : std::vector<int>{PLAYER_2, PLAYER_3, PLAYER_4})
    {
        auto &player = gamedata.playerArr[playerNum];

        if (player.passed)
            continue;

        if (player.bid == gamedata.roundInfo.bidAmount)
            continue;

        int prevBid = player.bid;
        player.bid = player.cpu->getBid();
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