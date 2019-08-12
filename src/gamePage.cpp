#include "GamePage.h"
#include "settings.h"

GamePage::GamePage(QWidget *parent) : QWidget(parent)
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

    applyButton = new QPushButton;
    applyButton->setText("Apply");
    QObject::connect(applyButton, &QAbstractButton::clicked, this, &GamePage::onApply);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(nestGroup);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(applyButton);
    mainLayout->addStretch(1);
    
    setLayout(mainLayout);
}

void GamePage::onApply()
{
    applyNest();
}

void GamePage::applyNest()
{
    // these settings will be taken into account the next time a player wins bid
    int numMiddleCardsAllowed = numMiddleCardsAllowedSpinBox->value();
    Settings::Game::writeNumCardsMiddleAllowed(numMiddleCardsAllowed);
}