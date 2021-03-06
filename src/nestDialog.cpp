#include "gameData.h"
#include "messageBox.h"
#include "nestDialog.h"
#include "settings.h"
#include "utils.h"

NestDialog::NestDialog(CardVector pOriginalNest, QWidget *parent) : originalNest(pOriginalNest), QDialogWithClickableCardArray(true, parent)
{
    setOriginalNestStyles("background-color: white; border: 2px solid");

    auto setupLabel = [this](ScaledQLabel *label, QString text, QPoint pos) {
        label->setParent(this);
        label->setFont(QFont("Times", 12));
        label->setText(text);
        label->move(pos);
    };

    auto setupPushButton = [this](ScaledQPushButton *pushButton, QString text, QSize size, QPoint pos) {
        pushButton->setParent(this);
        pushButton->setText(text);
        pushButton->move(pos);
        pushButton->resize(size.width(), size.height());
        pushButton->setFont(QFont("Times", 10));
    };

    nestCardsLabel = new ScaledQLabel;
    setupLabel(nestCardsLabel, "Middle cards (click to take)", {300, 10});

    nestCards = new ClickableCardArray(DRAW_POSITION_NEST_DLG_TOP, SIZE_SMALL, this);
    nestCards->showCards(gamedata.nest, &originalNestStyles);

    player1CardsPreviewLabel = new ScaledQLabel;
    setupLabel(player1CardsPreviewLabel, "New hand (click to discard)", {300, 235});

    player1CardsPreview = new ClickableCardArray(DRAW_POSITION_NEST_DLG_BOTTOM, SIZE_SMALL, this);
    player1CardsPreview->showCards(gamedata.playerArr[PLAYER_1].cardArr, &originalNestStyles);

    autoChooseNestButton = new ScaledQPushButton;
    setupPushButton(autoChooseNestButton, "Auto choose nest...", {125, 25}, {220, 450});

    resetNestButton = new ScaledQPushButton;
    setupPushButton(resetNestButton, "Reset nest...", {125, 25}, {350, 450});

    doneNestButton = new ScaledQPushButton;
    setupPushButton(doneNestButton, "Done nest...", {125, 25}, {480, 450});

    connect(autoChooseNestButton, &QPushButton::pressed, this, &NestDialog::autoChooseNestButtonPressed);
    connect(resetNestButton, &QPushButton::pressed, this, &NestDialog::resetNestButtonPressed);
    connect(doneNestButton, &QPushButton::pressed, this, &NestDialog::doneNestButtonPressed);

    highlightCardsCheckBox = new ScaledQCheckBox;
    highlightCardsCheckBox->setParent(this);
    highlightCardsCheckBox->setText("Highlight nest cards");
    highlightCardsCheckBox->move({700, 450});
    highlightCardsCheckBox->setFont(QFont("Times", 10));

    connect(highlightCardsCheckBox, &QCheckBox::pressed,
                     this, &NestDialog::highlightCardsCheckBoxPressed);

    resize(NEST_DIALOG_SIZE);
    setWindowTitle("Nest Dialog");
    setWindowIcon(QIcon(":rookicon.gif"));
    setStyleSheet("background-color: white");
}

void NestDialog::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto button : std::vector<ScaledQPushButton *>{autoChooseNestButton, resetNestButton, doneNestButton})
        button->rescale();

    for (auto label : std::vector<ScaledQLabel *>{nestCardsLabel, player1CardsPreviewLabel})
        label->rescale();

    for (auto clickableCardArray : std::vector<ClickableCardArray *>{nestCards, player1CardsPreview})
        clickableCardArray->rescale();

    for (auto checkBox : std::vector<ScaledQCheckBox *>{highlightCardsCheckBox})
        checkBox->rescale();
}

void NestDialog::reject()
{
    // do nothing
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