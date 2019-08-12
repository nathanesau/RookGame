#include "gameController.h"
#include "messageBox.h"
#include "nestDialog.h"
#include "settings.h"
#include "utils.h"

NestDialog::NestDialog(CardVector pOriginalNest, QMainWindow *pMainWindow, QWidget *parent) : originalNest(pOriginalNest),
                                                                                              mainWindow(pMainWindow),
                                                                                              QDialogWithClickableCardArray(true, parent)
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

    centerCardsLabel = new ScaledQLabel;
    setupLabel(centerCardsLabel, "Middle cards (click to take)", {300, 10});

    centerCards = new ClickableCardArray(DRAW_POSITION_NEST_DLG_TOP, SIZE_SMALL, this);
    centerCards->showCards(gc.data.nest, &originalNestStyles);

    bottomCardsPreviewLabel = new ScaledQLabel;
    setupLabel(bottomCardsPreviewLabel, "New hand (click to discard)", {300, 235});

    bottomCardsPreview = new ClickableCardArray(DRAW_POSITION_NEST_DLG_BOTTOM, SIZE_SMALL, this);
    bottomCardsPreview->showCards(gc.data.playerArr[PLAYER_1].cardArr, &originalNestStyles);

    autoChooseNestButton = new ScaledQPushButton;
    setupPushButton(autoChooseNestButton, "Auto choose nest...", {125, 25}, {220, 450});

    resetNestButton = new ScaledQPushButton;
    setupPushButton(resetNestButton, "Reset nest...", {125, 25}, {350, 450});

    doneNestButton = new ScaledQPushButton;
    setupPushButton(doneNestButton, "Done nest...", {125, 25}, {480, 450});

    QObject::connect(autoChooseNestButton, &QPushButton::pressed, this, &NestDialog::autoChooseNestButtonPressed);
    QObject::connect(resetNestButton, &QPushButton::pressed, this, &NestDialog::resetNestButtonPressed);
    QObject::connect(doneNestButton, &QPushButton::pressed, this, &NestDialog::doneNestButtonPressed);

    highlightCardsCheckBox = new ScaledQCheckBox;
    highlightCardsCheckBox->setParent(this);
    highlightCardsCheckBox->setText("Highlight nest cards");
    highlightCardsCheckBox->move({700, 450});
    highlightCardsCheckBox->setFont(QFont("Times", 10));

    QObject::connect(highlightCardsCheckBox, &QCheckBox::pressed,
                     this, &NestDialog::highlightCardsCheckBoxPressed);

    resize(NEST_DIALOG_SIZE);
    setWindowTitle("Nest Dialog");
    setWindowIcon(QIcon(":rookicon.gif"));
    setStyleSheet("background-color: white");
    #ifdef WINDOW_ALWAYS_ON_TOP
    setWindowFlags(Qt::WindowStaysOnTopHint);
    #endif
}

void NestDialog::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto button : vector<ScaledQPushButton *>{autoChooseNestButton, resetNestButton, doneNestButton})
        button->rescale();

    for (auto label : vector<ScaledQLabel *>{centerCardsLabel, bottomCardsPreviewLabel})
        label->rescale();

    for (auto clickableCardArray : vector<ClickableCardArray *>{centerCards, bottomCardsPreview})
        clickableCardArray->rescale();

    for (auto checkBox : vector<ScaledQCheckBox *>{highlightCardsCheckBox})
        checkBox->rescale();
}

void NestDialog::reject()
{
    // do nothing
}

void NestDialog::setOriginalNestStyles(string style)
{
    for (auto card : originalNest)
    {
        originalNestStyles[card] = style;
    }
}

void NestDialog::onCardClicked(ClickableCard *clickableCard)
{
    Card card = clickableCard->data;

    auto &nest = gc.data.nest;
    auto &hand = gc.data.playerArr[PLAYER_1].cardArr;

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

    centerCards->showCards(gc.data.nest, &originalNestStyles);
    bottomCardsPreview->showCards(gc.data.playerArr[PLAYER_1].cardArr, &originalNestStyles);
}

void NestDialog::onCardHoverEnter(ClickableCard *clickableCard)
{
    // do nothing
}

void NestDialog::onCardHoverLeave(ClickableCard *clickableCard)
{
    // do nothing
}

void NestDialog::autoChooseNest()
{
    CardVector &nest = gc.data.nest;
    CardVector &cardArr = gc.data.playerArr[PLAYER_1].cardArr;

    CardVector newNest;
    CardVector newCardArr;
    newCardArr.append({&cardArr, &nest});

    auto suitInfoArr = newCardArr.getSuitInfoArray();

    int cardsToRemove = 5;

    while (cardsToRemove > 0) // discard worst cards to nest
    {
        auto It = --suitInfoArr.end();

        if (It->count > 0)
        {
            int n = min(It->count, cardsToRemove);
            auto cardsRemoved = newCardArr.removeThisSuit(It->suit, n);
            cardsToRemove -= n;

            for (auto &card : cardsRemoved)
            {
                newNest.push_back(card);
            }
        }

        suitInfoArr.erase(It);
    }

    cardArr = newCardArr;
    nest = newNest;
}

void NestDialog::autoChooseNestButtonPressed()
{
    NestDialog::autoChooseNest();

    centerCards->showCards(gc.data.nest, &originalNestStyles);
    bottomCardsPreview->showCards(gc.data.playerArr[PLAYER_1].cardArr, &originalNestStyles);
}

void NestDialog::resetNestButtonPressed()
{
    gc.data.playerArr[PLAYER_1].cardArr.append({&gc.data.nest});
    gc.data.playerArr[PLAYER_1].cardArr.remove(originalNest);
    gc.data.playerArr[PLAYER_1].cardArr.sort();

    gc.data.nest = originalNest;
    gc.data.nest.sort();

    centerCards->showCards(gc.data.nest, &originalNestStyles);
    bottomCardsPreview->showCards(gc.data.playerArr[PLAYER_1].cardArr, &originalNestStyles);
}

void NestDialog::doneNestButtonPressed()
{
    if (gc.data.nest.size() != 5)
    {
        MessageBox msgBox;
        msgBox.setText("Nest must have exactly 5 cards");
        msgBox.setWindowTitle("Nest problem");
        Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
        msgBox.exec();
    }
    else // 5 cards left in nest
    {
        auto &newHand = gc.data.playerArr[PLAYER_1].cardArr;
        int numMiddleCardsAllowed = Settings::Game::readNumCardsMiddleAllowed();
        int numMiddleCardsSelected = accumulate(newHand.begin(), newHand.end(), 0, [this](int a, const Card &b) {
            return originalNest.hasCard(b) ? a + 1 : a;
        });

        if (numMiddleCardsSelected > numMiddleCardsAllowed) // too many nest cards selected
        {
            string msg = to_string(numMiddleCardsSelected) + " are selected but only " + to_string(numMiddleCardsAllowed) +
                         " are allowed to be selected.\n\nReview selected cards.\n\nTip: Use \"Highlight nest cards\" to see selected cards.";

            MessageBox msgBox;
            msgBox.setText(QString::fromStdString(msg));
            msgBox.setWindowTitle("Nest problem");
            Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
            msgBox.exec();
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

    centerCards->showCards(gc.data.nest, &originalNestStyles);
    bottomCardsPreview->showCards(gc.data.playerArr[PLAYER_1].cardArr, &originalNestStyles);
}