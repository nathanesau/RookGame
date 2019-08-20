#include "gameData.h"
#include "gameDatabase.h"
#include "saveSlotPage.h"

SaveSlotPage::SaveSlotPage(QString dbName, QWidget *parent) : QWidget(parent)
{
    db.LoadFromDb(dbName, pageData);

    existsLabel = new QLabel;
    existsLabel->setText("File exists"); // should exist unless user deletes slot db files

    existsGroup = new QGroupBox;
    existsGroup->setTitle("File Info");

    existsLayout = new QHBoxLayout;
    existsLayout->addWidget(existsLabel);
    existsGroup->setLayout(existsLayout);

    auto setupScoreLabel = [this](QLabel *scoreLabel, int playerNum) {
        string labelText = pageData.playerArr[playerNum].getPlayerName() + ": " +
                           to_string(pageData.overallInfo.playerScores[playerNum]);

        scoreLabel->setText(QString::fromStdString(labelText));
    };

    score1Label = new QLabel;
    setupScoreLabel(score1Label, PLAYER_1);

    score2Label = new QLabel;
    setupScoreLabel(score2Label, PLAYER_2);

    score3Label = new QLabel;
    setupScoreLabel(score3Label, PLAYER_3);

    score4Label = new QLabel;
    setupScoreLabel(score4Label, PLAYER_4);

    scoresGroup = new QGroupBox;
    scoresGroup->setTitle("Scores");

    scoresLayout = new QVBoxLayout;
    scoresLayout->addWidget(score1Label);
    scoresLayout->addWidget(score2Label);
    scoresLayout->addWidget(score3Label);
    scoresLayout->addWidget(score4Label);
    scoresGroup->setLayout(scoresLayout);

    auto setupTrumpLabel = [this](QLabel *trumpLabel) {
        int trump = pageData.roundInfo.trump;
        string trumpStr = (trump == SUIT_UNDEFINED) ? "???" : Card(trump, VALUE_UNDEFINED).getSuitAsString();

        if (!trumpStr.empty())
        {
            trumpStr[0] = toupper(trumpStr[0]);
        }

        string labelText = "Trump: " + trumpStr;
        QString labelTextQt = QString::fromStdString(labelText);
        trumpLabel->setText(labelTextQt);
    };

    trumpLabel = new QLabel;
    setupTrumpLabel(trumpLabel);

    auto setupBidLabel = [this](QLabel *bidLabel) {
        int bid = pageData.roundInfo.bidAmount;
        string bidStr = (bid == 0) ? "???" : to_string(pageData.roundInfo.bidAmount);
        string labelText = "Bid: " + bidStr;
        bidLabel->setText(QString::fromStdString(labelText));
    };

    bidLabel = new QLabel;
    setupBidLabel(bidLabel);

    auto setupPartnerLabel = [this](QLabel *partnerLabel) {
        string labelText = "Partner: " + pageData.roundInfo.partnerCard.getCardAsString();
        QString labelTextQt = QString::fromStdString(labelText);
        partnerLabel->setText(labelTextQt);
    };

    partnerLabel = new QLabel;
    setupPartnerLabel(partnerLabel);

    roundGroup = new QGroupBox;
    roundGroup->setTitle("Current Round Info");

    roundLayout = new QVBoxLayout;
    roundLayout->addWidget(trumpLabel);
    roundLayout->addWidget(bidLabel);
    roundLayout->addWidget(partnerLabel);
    roundGroup->setLayout(roundLayout);

    auto setupCardLabel = [this](QLabel *cardLabel, int playerNum) {
        string labelText = pageData.playerArr[playerNum].getPlayerName() + ": " + 
                           pageData.handInfo.cardPlayed[playerNum].getCardAsString();
        QString labelTextQt = QString::fromStdString(labelText);
        cardLabel->setText(labelTextQt);
    };

    player1CardLabel = new QLabel;
    setupCardLabel(player1CardLabel, PLAYER_1);

    player2CardLabel = new QLabel;
    setupCardLabel(player2CardLabel, PLAYER_2);

    player3CardLabel = new QLabel;
    setupCardLabel(player3CardLabel, PLAYER_3);

    player4CardLabel = new QLabel;
    setupCardLabel(player4CardLabel, PLAYER_4);

    handGroup = new QGroupBox;
    handGroup->setTitle("Current Hand Info (Cards Played)");

    handLayout = new QVBoxLayout;
    handLayout->addWidget(player1CardLabel);
    handLayout->addWidget(player2CardLabel);
    handLayout->addWidget(player3CardLabel);
    handLayout->addWidget(player4CardLabel);
    handGroup->setLayout(handLayout);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(existsGroup);
    mainLayout->addWidget(scoresGroup);
    mainLayout->addWidget(roundGroup);
    mainLayout->addWidget(handGroup);

    setLayout(mainLayout);
}
