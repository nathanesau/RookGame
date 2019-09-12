#include "gameData.h"
#include "messageBox.h"
#include "middleDialog.h"
#include "nestDialog.h"
#include "partnerDialog.h"
#include "trumpDialog.h"
#include "utils.h"

MiddleDialog::MiddleDialog(QWidget *parent) : QDialogWithClickableCardArray(true, parent),
                                              originalNest(gamedata.nest)
{
    ui.setupUi(this);

    nestCards = new ClickableCardArray(DRAW_POSITION_MIDDLE_DLG_NEST, SIZE_TINY, this);
    nestCards->showCards(gamedata.nest);

    partnerCards = new ClickableCardArray(DRAW_POSITION_MIDDLE_DLG_PARTNER, SIZE_TINY, this);
    partnerCards->showCards({Card(SUIT_UNDEFINED, VALUE_UNDEFINED)});

    connect(ui.selectNestButton, &QPushButton::pressed, this, &MiddleDialog::onSelectNestButtonPressed);
    connect(ui.autoSelectNestButton, &QPushButton::pressed, this, &MiddleDialog::onAutoSelectNestButtonPressed);
    connect(ui.selectTrumpButton, &QPushButton::pressed, this, &MiddleDialog::onSelectTrumpButtonPressed);
    connect(ui.autoSelectTrumpButton, &QPushButton::pressed, this, &MiddleDialog::onAutoSelectTrumpButtonPressed);
    connect(ui.selectPartnerButton, &QPushButton::pressed, this, &MiddleDialog::onSelectPartnerButtonPressed);
    connect(ui.autoSelectPartnerButton, &QPushButton::pressed, this, &MiddleDialog::onAutoSelectPartnerButtonPressed);
    connect(ui.okButton, &QPushButton::pressed, this, &MiddleDialog::onOkButtonPressed);

    resize(MIDDLE_DIALOG_SIZE);
    setWindowIcon(QIcon(":rookicon.gif"));
    setStyleSheet("background-color: white");
}

void MiddleDialog::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto clickableCardArray : std::vector<ClickableCardArray *>{nestCards, partnerCards})
        clickableCardArray->rescale();

    for (auto label : std::vector<ScaledQLabel *>{ui.trumpCategoryLabel, ui.partnerCategoryLabel, ui.trumpLabel,
                                                  ui.nestCategoryLabel})
        label->rescale();

    for (auto button : std::vector<ScaledQPushButton *>{ui.selectPartnerButton, ui.selectTrumpButton, ui.okButton,
                                                        ui.autoSelectTrumpButton, ui.autoSelectPartnerButton,
                                                        ui.autoSelectNestButton})
        button->rescale();
}

void MiddleDialog::reject()
{
    // do nothing
}

void MiddleDialog::onCardClicked(ClickableCard *clickableCard)
{
    // do nothing
}

void MiddleDialog::onSelectNestButtonPressed()
{
    // set gamedata.nest
    emit showNestDialog(originalNest);

    nestCards->showCards(gamedata.nest);

    // refresh nest, player 1 cards
    emit refreshCardWidgets(gamedata);
}

void MiddleDialog::onAutoSelectNestButtonPressed()
{
    gamedata.playerArr[PLAYER_1].cpu->selectNest();

    nestCards->showCards(gamedata.nest);

    // refresh nest, player 1 cards
    emit refreshCardWidgets(gamedata);
}

void MiddleDialog::onSelectTrumpButtonPressed()
{
    // set gamedata.roundInfo.trump
    emit showTrumpDialog();

    setupTrumpLabel(gamedata.roundInfo.trump);
}

void MiddleDialog::onAutoSelectTrumpButtonPressed()
{
    gamedata.playerArr[PLAYER_1].cpu->selectTrump();

    setupTrumpLabel(gamedata.roundInfo.trump);
}

void MiddleDialog::onSelectPartnerButtonPressed()
{
    // set gamedata.roundInfo.partnerCard
    emit showPartnerDialog();

    partnerCards->showCards({gamedata.roundInfo.partnerCard});
}

void MiddleDialog::onAutoSelectPartnerButtonPressed()
{
    gamedata.playerArr[PLAYER_1].cpu->selectPartner();

    partnerCards->showCards({gamedata.roundInfo.partnerCard});
}

void MiddleDialog::onOkButtonPressed()
{
    if (gamedata.roundInfo.trump == SUIT_UNDEFINED || gamedata.roundInfo.partnerCard.isUndefined())
    {
        emit showInvalidMiddleDialogMessage();
    }
    else
    {
        QDialog::accept();
    }
}

void MiddleDialog::setupTrumpLabel(int suit)
{
    auto trumpLabel = ui.trumpLabel;

    switch (suit)
    {
    case SUIT_BLACK:
        trumpLabel->setStyleSheet("background-color: black; color: white");
        trumpLabel->setText("Black");
        break;
    case SUIT_GREEN:
        trumpLabel->setStyleSheet("background-color: green");
        trumpLabel->setText("Green");
        break;
    case SUIT_RED:
        trumpLabel->setStyleSheet("background-color: red");
        trumpLabel->setText("Red");
        break;
    case SUIT_YELLOW:
        trumpLabel->setStyleSheet("background-color: yellow");
        trumpLabel->setText("Yellow");
        break;
    default:
        trumpLabel->setStyleSheet(""); // empty style
        trumpLabel->setText("No Trump Selected");
        break;
    }
}
