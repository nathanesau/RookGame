#include "gameData.h"
#include "messageBox.h"
#include "nestDialog.h"
#include "settings.h"
#include "utils.h"

void NestDialog::initializeMiddleCardsLayout()
{
    QFont labelFont;
    labelFont.setPointSize(12);

    nestCardsLabel = new QLabel;
    nestCardsLabel->setText("Middle cards (click to take)");
    nestCardsLabel->setAlignment(Qt::AlignHCenter);
    nestCardsLabel->setFont(labelFont);

    nestCards = new LayoutClickableCardArray(DRAW_POSITION_NEST_DLG_TOP, SIZE_SMALL);
    connect(nestCards, &LayoutClickableCardArray::clicked, this, &NestDialog::onCardClicked);

    middleCardsLayout = new QVBoxLayout;
    middleCardsLayout->addWidget(nestCardsLabel, 20);
    middleCardsLayout->addWidget(nestCards, 80);
}

void NestDialog::initializeNewHandLayout()
{
    QFont labelFont;
    labelFont.setPointSize(12);

    player1CardsPreviewLabel = new QLabel;
    player1CardsPreviewLabel->setText("New hand (click to discard)");
    player1CardsPreviewLabel->setAlignment(Qt::AlignHCenter);
    player1CardsPreviewLabel->setFont(labelFont);

    player1CardsPreview = new LayoutClickableCardArray(DRAW_POSITION_NEST_DLG_BOTTOM, SIZE_SMALL);
    connect(player1CardsPreview, &LayoutClickableCardArray::clicked, this, &NestDialog::onCardClicked);

    newHandLayout = new QVBoxLayout;
    newHandLayout->addWidget(player1CardsPreviewLabel, 20);
    newHandLayout->addWidget(player1CardsPreview, 80);
}

void NestDialog::initializeButtonsCheckBoxLayout()
{
    QFont buttonFont;
    buttonFont.setPointSize(10);

    QFont checkBoxFont;
    checkBoxFont.setPointSize(10);

    autoChooseNestButton = new QPushButton;
    autoChooseNestButton->setText("Auto choose nest...");
    autoChooseNestButton->setFont(buttonFont);
    connect(autoChooseNestButton, &QPushButton::pressed, this, &NestDialog::autoChooseNestButtonPressed);

    resetNestButton = new QPushButton;
    resetNestButton->setText("Reset nest...");
    resetNestButton->setFont(buttonFont);
    connect(resetNestButton, &QPushButton::pressed, this, &NestDialog::resetNestButtonPressed);

    doneNestButton = new QPushButton;
    doneNestButton->setText("Done nest...");
    doneNestButton->setFont(buttonFont);
    connect(doneNestButton, &QPushButton::pressed, this, &NestDialog::doneNestButtonPressed);

    highlightCardsCheckBox = new QCheckBox;
    highlightCardsCheckBox->setText("Highlight nest cards");
    highlightCardsCheckBox->setFont(checkBoxFont);
    connect(highlightCardsCheckBox, &QCheckBox::pressed, this, &NestDialog::highlightCardsCheckBoxPressed);

    buttonsCheckBoxLayout = new QHBoxLayout;
    buttonsCheckBoxLayout->addStretch(1);
    buttonsCheckBoxLayout->addWidget(autoChooseNestButton);
    buttonsCheckBoxLayout->addWidget(resetNestButton);
    buttonsCheckBoxLayout->addWidget(doneNestButton);
    buttonsCheckBoxLayout->addStretch(1);
    buttonsCheckBoxLayout->addWidget(highlightCardsCheckBox);
}

NestDialog::NestDialog(CardVector pOriginalNest, QWidget *parent) : originalNest(pOriginalNest), QDialog(parent)
{
    setOriginalNestStyles("background-color: white; border: 2px solid");

    initializeMiddleCardsLayout();
    initializeNewHandLayout();
    initializeButtonsCheckBoxLayout();

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(middleCardsLayout);
    mainLayout->addLayout(newHandLayout);
    mainLayout->addLayout(buttonsCheckBoxLayout);

    nestCards->showCards(gamedata.nest, &originalNestStyles);
    player1CardsPreview->showCards(gamedata.playerArr[PLAYER_1].cardArr, &originalNestStyles);

    resize(NEST_DIALOG_SIZE);
    setLayout(mainLayout);
    setWindowTitle("Nest Dialog");
    setWindowIcon(QIcon(":rookicon.gif"));
    setStyleSheet("background-color: white");
}

void NestDialog::setOriginalNestStyles(std::string style)
{
    for (auto card : originalNest)
    {
        originalNestStyles[card] = style;
    }
}

void NestDialog::onCardClicked(ClickableCard *clickableCard)
{
    Card card = clickableCard->data;

    auto &nest = gamedata.nest;
    auto &hand = gamedata.playerArr[PLAYER_1].cardArr;

    auto nestIt = std::find(nest.begin(), nest.end(), card);
    auto handIt = std::find(hand.begin(), hand.end(), card);

    if (nestIt != nest.end())
    {
        nest.erase(nestIt);

        hand.push_back(card);
        hand.sort();
    }
    else // handIt != hand.end()
    {
        hand.erase(handIt);
        nest.push_back(card);
        nest.sort();
    }

    nestCards->showCards(gamedata.nest, &originalNestStyles);
    player1CardsPreview->showCards(gamedata.playerArr[PLAYER_1].cardArr, &originalNestStyles);
}

void NestDialog::autoChooseNestButtonPressed()
{
    gamedata.playerArr[PLAYER_1].cpu->selectNest();

    nestCards->showCards(gamedata.nest, &originalNestStyles);
    player1CardsPreview->showCards(gamedata.playerArr[PLAYER_1].cardArr, &originalNestStyles);
}

void NestDialog::resetNestButtonPressed()
{
    gamedata.playerArr[PLAYER_1].cardArr.append(gamedata.nest);
    gamedata.playerArr[PLAYER_1].cardArr.remove(originalNest);
    gamedata.playerArr[PLAYER_1].cardArr.sort();

    gamedata.nest = originalNest;
    gamedata.nest.sort();

    nestCards->showCards(gamedata.nest, &originalNestStyles);
    player1CardsPreview->showCards(gamedata.playerArr[PLAYER_1].cardArr, &originalNestStyles);
}

void NestDialog::doneNestButtonPressed()
{
    if (gamedata.nest.size() != 5)
    {
        emit showWrongNumNestCardsMessage();
    }
    else // 5 cards left in nest
    {
        auto &newHand = gamedata.playerArr[PLAYER_1].cardArr;
        int numMiddleCardsAllowed = Settings::Game::readNumCardsMiddleAllowed();
        int numMiddleCardsSelected = accumulate(newHand.begin(), newHand.end(), 0, [this](int a, const Card &b) {
            return originalNest.hasCard(b) ? a + 1 : a;
        });

        if (numMiddleCardsSelected > numMiddleCardsAllowed) // too many nest cards selected
        {
            emit showTooManyNestCardsMessage(numMiddleCardsAllowed, numMiddleCardsSelected);
        }
        else
        {
            QDialog::accept();
        }
    }
}

void NestDialog::highlightCardsCheckBoxPressed()
{
    if (!highlightCardsCheckBox->isChecked())
    {
        setOriginalNestStyles("background-color: cyan; border: 2px solid");
    }
    else
    {
        setOriginalNestStyles("background-color: white; border: 2px solid");
    }

    nestCards->showCards(gamedata.nest, &originalNestStyles);
    player1CardsPreview->showCards(gamedata.playerArr[PLAYER_1].cardArr, &originalNestStyles);
}