#include "gameData.h"
#include "partnerDialog.h"
#include "settings.h"

PartnerDialogLabel::PartnerDialogLabel(PartnerDialog *parent) : ScaledQLabel(parent)
{
}

void PartnerDialogLabel::mousePressEvent(QMouseEvent *event)
{
    PartnerDialog *parentDialog = dynamic_cast<PartnerDialog *>(parent());

    parentDialog->onPartnerLabelClicked(this);
}

PartnerDialog::PartnerDialog(Card &pCardSelected, QWidget *parent) : cardSelected(pCardSelected),
                                                                     ScaledQDialog(true, parent)
{
    setupCardArrays();

    auto setupPartnerLabel = [this](PartnerDialogLabel *label, QString text, QString style, QPoint pos) {
        label->setFont(QFont("Times", 12));
        label->setText(text);
        label->setStyleSheet(style);
        label->setParent(this);
        label->move(pos);
        label->setAlignment(Qt::AlignCenter);
        label->resize(50, 25);
    };

    partnerOptionsCards = new ClickableCardArray(DRAW_POSITION_PARTNER_DLG, SIZE_TINY, this);
    connect(partnerOptionsCards, &ClickableCardArray::clicked, this, &PartnerDialog::onCardClicked);

    redLabel = new PartnerDialogLabel;
    setupPartnerLabel(redLabel, "Red", "background-color: red; border: 2px solid", QPoint(25, 25));

    blackLabel = new PartnerDialogLabel;
    setupPartnerLabel(blackLabel, "Black", "background-color: black; color: white; border: 2px solid", QPoint(75, 25));

    greenLabel = new PartnerDialogLabel;
    setupPartnerLabel(greenLabel, "Green", "background-color: green; border: 2px solid", QPoint(125, 25));

    yellowLabel = new PartnerDialogLabel;
    setupPartnerLabel(yellowLabel, "Yellow", "background-color: yellow; border: 2px solid", QPoint(175, 25));

    wildLabel = new PartnerDialogLabel;
    setupPartnerLabel(wildLabel, "Wild", "background-color: white; border:2px solid", QPoint(225, 25));

    cancelButton = new ScaledQPushButton;
    cancelButton->setParent(this);
    cancelButton->setText("Cancel");
    cancelButton->setFont(QFont("Times", 10));
    cancelButton->resize(50, 25);
    cancelButton->move({700, 250});

    connect(cancelButton, &QPushButton::pressed, this, &QDialog::accept);

    setWindowTitle("Click partner card...");
    setWindowIcon(QIcon(":rookicon.gif"));
    resize(PARTNER_DIALOG_SIZE);
}

void PartnerDialog::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto label : std::vector<PartnerDialogLabel *>{blackLabel, greenLabel, redLabel, yellowLabel, wildLabel})
        label->rescale();

    for (auto button : std::vector<ScaledQPushButton *>{cancelButton})
        button->rescale();

    for (auto clickableCardArray : std::vector<ClickableCardArray *>{partnerOptionsCards})
        clickableCardArray->rescale();
}

void PartnerDialog::reject()
{
    // do nothing
}

void PartnerDialog::onPartnerLabelClicked(PartnerDialogLabel *label)
{
    using SuitMap = std::map<std::string, CardVector *>;

    SuitMap suitMap = {{"Black", &blackCards},
                       {"Green", &greenCards},
                       {"Red", &redCards},
                       {"Yellow", &yellowCards},
                       {"Wild", &wildCards}};

    partnerOptionsCards->showCards(*suitMap[label->text().toStdString()]);
}

void PartnerDialog::onCardClicked(ClickableCard *clickableCard)
{
    cardSelected.suit = clickableCard->data.suit;
    cardSelected.value = clickableCard->data.value;

    QDialog::accept();
}

void PartnerDialog::setupCardArrays()
{
    CardVector aggregateCardArr;

    if (Settings::Game::readPickNestAsPartner())
    {
        aggregateCardArr.append(gamedata.nest);
    }

    if (Settings::Game::readPickSelfAsPartner())
    {
        aggregateCardArr.append(gamedata.playerArr[PLAYER_1].cardArr);
    }

    aggregateCardArr.append(gamedata.playerArr[PLAYER_2].cardArr);
    aggregateCardArr.append(gamedata.playerArr[PLAYER_3].cardArr);
    aggregateCardArr.append(gamedata.playerArr[PLAYER_4].cardArr);
    aggregateCardArr.sort();

    copy_if(aggregateCardArr.begin(), aggregateCardArr.end(), back_inserter(blackCards), [](auto &c) { return c.suit == SUIT_BLACK; });
    copy_if(aggregateCardArr.begin(), aggregateCardArr.end(), back_inserter(greenCards), [](auto &c) { return c.suit == SUIT_GREEN; });
    copy_if(aggregateCardArr.begin(), aggregateCardArr.end(), back_inserter(redCards), [](auto &c) { return c.suit == SUIT_RED; });
    copy_if(aggregateCardArr.begin(), aggregateCardArr.end(), back_inserter(yellowCards), [](auto &c) { return c.suit == SUIT_YELLOW; });
    copy_if(aggregateCardArr.begin(), aggregateCardArr.end(), back_inserter(wildCards), [](auto &c) { return c.suit == SUIT_SPECIAL; });
}