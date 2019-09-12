#include "gamePage.h"
#include "settings.h"

void GamePage::initializeBidGroup()
{
    cpuBidAggressionLabel = new QLabel;
    cpuBidAggressionLabel->setText("Choose CPU bid aggression level: ");

    cpuBidAggressionBox = new QComboBox;
    cpuBidAggressionBox->addItems({"Low", "Medium", "High"});
    cpuBidAggressionBox->setCurrentIndex(Settings::Game::readCpuBidAggressionLevel());

    bidGroup = new QGroupBox;
    bidGroup->setTitle("Bid");

    bidLayout = new QHBoxLayout;
    bidLayout->addWidget(cpuBidAggressionLabel);
    bidLayout->addWidget(cpuBidAggressionBox);
    bidGroup->setLayout(bidLayout);
}

void GamePage::initializeNestGroup()
{
    numMiddleCardsAllowedLabel = new QLabel;
    numMiddleCardsAllowedLabel->setText(tr("Choose # cards allowed (%1 to %2): ").arg(3).arg(5));

    numMiddleCardsAllowedSpinBox = new QSpinBox;
    numMiddleCardsAllowedSpinBox->setRange(3, 5);
    numMiddleCardsAllowedSpinBox->setSingleStep(1);
    numMiddleCardsAllowedSpinBox->setValue(Settings::Game::readNumCardsMiddleAllowed());

    nestGroup = new QGroupBox;
    nestGroup->setTitle("Nest");

    nestLayout = new QHBoxLayout;
    nestLayout->addWidget(numMiddleCardsAllowedLabel);
    nestLayout->addWidget(numMiddleCardsAllowedSpinBox);
    nestGroup->setLayout(nestLayout);
}

void GamePage::initializePartnerGroup()
{
    pickSelfAsPartnerCheckBox = new QCheckBox;
    pickSelfAsPartnerCheckBox->setText("Allow pick self as partner");
    pickSelfAsPartnerCheckBox->setChecked(Settings::Game::readPickSelfAsPartner());

    pickNestAsPartnerCheckBox = new QCheckBox;
    pickNestAsPartnerCheckBox->setText("Allow pick nest as partner");
    pickNestAsPartnerCheckBox->setChecked(Settings::Game::readPickNestAsPartner());

    partnerGroup = new QGroupBox;
    partnerGroup->setTitle("Partner");

    partnerLayout = new QVBoxLayout;
    partnerLayout->addWidget(pickSelfAsPartnerCheckBox);
    partnerLayout->addWidget(pickNestAsPartnerCheckBox);
    partnerGroup->setLayout(partnerLayout);
}

void GamePage::initializeCpuGroup()
{
    cpuRandomnessLabel = new QLabel;
    cpuRandomnessLabel->setText("Choose CPU randomness level: ");

    cpuRandomnessBox = new QComboBox;
    cpuRandomnessBox->addItems({"None (0%)", "Low (10%)", "Medium (20%)", "High (30%)"});
    cpuRandomnessBox->setToolTip("Set the randomness for when CPU plays cards");
    cpuRandomnessBox->setCurrentIndex(Settings::Game::readCpuRandomnessLevel());    

    cpuGroup = new QGroupBox;
    cpuGroup->setTitle("CPU settings");
    
    cpuLayout = new QHBoxLayout;
    cpuLayout->addWidget(cpuRandomnessLabel);
    cpuLayout->addWidget(cpuRandomnessBox);
    cpuGroup->setLayout(cpuLayout);
}

GamePage::GamePage(QWidget *parent) : QWidget(parent)
{
    initializeBidGroup();
    initializeNestGroup();
    initializePartnerGroup();
    initializeCpuGroup();

    applyButton = new QPushButton;
    applyButton->setText("Apply");
    connect(applyButton, &QAbstractButton::clicked, this, &GamePage::onApply);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(bidGroup);
    mainLayout->addWidget(nestGroup);
    mainLayout->addWidget(partnerGroup);
    mainLayout->addWidget(cpuGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(applyButton);
    mainLayout->addStretch(1);
    
    setLayout(mainLayout);
}

void GamePage::onApply()
{
    applyBidGroup();
    applyNestGroup();
    applyPartnerGroup();
    applyCpuGroup();
}

void GamePage::applyBidGroup()
{
    int cpuBidAggressionLevel = cpuBidAggressionBox->currentIndex();
    Settings::Game::writeCpuBidAggressionLevel(cpuBidAggressionLevel);
}

void GamePage::applyNestGroup()
{
    // these settings will be taken into account the next time a player wins bid
    int numMiddleCardsAllowed = numMiddleCardsAllowedSpinBox->value();
    Settings::Game::writeNumCardsMiddleAllowed(numMiddleCardsAllowed);
}

void GamePage::applyPartnerGroup()
{
    bool allowPickSelfAsPartner = pickSelfAsPartnerCheckBox->isChecked();
    Settings::Game::writePickSelfAsPartner(allowPickSelfAsPartner);

    bool allowPickNestAsPartner = pickNestAsPartnerCheckBox->isChecked();
    Settings::Game::writePickNestAsPartner(allowPickNestAsPartner);
}

void GamePage::applyCpuGroup()
{
    int cpuRandomnessLevel = cpuRandomnessBox->currentIndex();
    Settings::Game::writeCpuRandomnessLevel(cpuRandomnessLevel);
}