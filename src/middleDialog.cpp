#include "gameData.h"
#include "messageBox.h"
#include "middleDialog.h"
#include "nestDialog.h"
#include "partnerDialog.h"
#include "trumpDialog.h"
#include "utils.h"

void MiddleDialog::setupUi()
{   
    QFont categoryFont;
    categoryFont.setPointSize(12);
    categoryFont.setBold(true);
    categoryFont.setWeight(75);

    QFont buttonFont;
    buttonFont.setPointSize(10);

    QFont labelFont;
    labelFont.setPointSize(10);

    trumpCategoryLabel = new ScaledQLabel;
    trumpCategoryLabel->setText("Trump");
    trumpCategoryLabel->setParent(this);
    trumpCategoryLabel->setGeometry(QRect(90, 160, 121, 31));
    trumpCategoryLabel->setFont(categoryFont);

    selectPartnerButton = new ScaledQPushButton;
    selectPartnerButton->setText("Select partner...");
    selectPartnerButton->setParent(this);
    selectPartnerButton->setGeometry(QRect(260, 270, 141, 28));
    selectPartnerButton->setFont(buttonFont);

    partnerCategoryLabel = new ScaledQLabel;
    partnerCategoryLabel->setText("Partner");
    partnerCategoryLabel->setParent(this);
    partnerCategoryLabel->setGeometry(QRect(90, 270, 131, 31));
    partnerCategoryLabel->setFont(categoryFont);

    selectTrumpButton = new ScaledQPushButton;
    selectTrumpButton->setText("Select trump...");
    selectTrumpButton->setParent(this);
    selectTrumpButton->setGeometry(QRect(260, 160, 141, 31));
    selectTrumpButton->setFont(buttonFont);
    
    trumpLabel = new ScaledQLabel;
    trumpLabel->setText("No Trump Selected");
    trumpLabel->setParent(this);
    trumpLabel->setGeometry(QRect(510, 150, 121, 51));
    trumpLabel->setAlignment(Qt::AlignCenter);
    trumpLabel->setFont(labelFont);

    okButton = new ScaledQPushButton;
    okButton->setText("OK");
    okButton->setParent(this);
    okButton->setGeometry(QRect(620, 390, 75, 23));
    okButton->setFont(buttonFont);

    autoSelectTrumpButton = new ScaledQPushButton;
    autoSelectTrumpButton->setText("Auto select nest...");
    autoSelectTrumpButton->setParent(this);
    autoSelectTrumpButton->setGeometry(QRect(260, 190, 141, 31));
    autoSelectTrumpButton->setFont(buttonFont);

    autoSelectPartnerButton = new ScaledQPushButton;
    autoSelectPartnerButton->setText("Auto select partner...");
    autoSelectPartnerButton->setParent(this);
    autoSelectPartnerButton->setGeometry(QRect(260, 300, 141, 28));
    autoSelectPartnerButton->setFont(buttonFont);

    nestCategoryLabel = new ScaledQLabel;
    nestCategoryLabel->setText("Nest");
    nestCategoryLabel->setParent(this);
    nestCategoryLabel->setGeometry(QRect(90, 40, 121, 31));
    nestCategoryLabel->setFont(categoryFont);

    selectNestButton = new ScaledQPushButton;
    selectNestButton->setText("Select nest...");
    selectNestButton->setParent(this);
    selectNestButton->setGeometry(QRect(260, 40, 141, 31));
    selectNestButton->setFont(buttonFont);

    autoSelectNestButton = new ScaledQPushButton;
    autoSelectNestButton->setText("Auto select nest...");
    autoSelectNestButton->setParent(this);
    autoSelectNestButton->setGeometry(QRect(260, 70, 141, 31));
    autoSelectNestButton->setFont(buttonFont);
}

MiddleDialog::MiddleDialog(QWidget *parent) : ScaledQDialog(true, parent),
                                              originalNest(gamedata.nest)
{
    setupUi();

    nestCards = new ClickableCardArray(DRAW_POSITION_MIDDLE_DLG_NEST, SIZE_TINY, this);
    nestCards->showCards(gamedata.nest);

    partnerCards = new ClickableCardArray(DRAW_POSITION_MIDDLE_DLG_PARTNER, SIZE_TINY, this);
    partnerCards->showCards({Card(SUIT_UNDEFINED, VALUE_UNDEFINED)});

    connect(selectNestButton, &QPushButton::pressed, this, &MiddleDialog::onSelectNestButtonPressed);
    connect(autoSelectNestButton, &QPushButton::pressed, this, &MiddleDialog::onAutoSelectNestButtonPressed);
    connect(selectTrumpButton, &QPushButton::pressed, this, &MiddleDialog::onSelectTrumpButtonPressed);
    connect(autoSelectTrumpButton, &QPushButton::pressed, this, &MiddleDialog::onAutoSelectTrumpButtonPressed);
    connect(selectPartnerButton, &QPushButton::pressed, this, &MiddleDialog::onSelectPartnerButtonPressed);
    connect(autoSelectPartnerButton, &QPushButton::pressed, this, &MiddleDialog::onAutoSelectPartnerButtonPressed);
    connect(okButton, &QPushButton::pressed, this, &MiddleDialog::onOkButtonPressed);

    resize(MIDDLE_DIALOG_SIZE);
    setWindowIcon(QIcon(":rookicon.gif"));
}

void MiddleDialog::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto clickableCardArray : std::vector<ClickableCardArray *>{nestCards, partnerCards})
        clickableCardArray->rescale();

    for (auto label : std::vector<ScaledQLabel *>{trumpCategoryLabel, partnerCategoryLabel, trumpLabel,
                                                  nestCategoryLabel})
        label->rescale();

    for (auto button : std::vector<ScaledQPushButton *>{selectPartnerButton, selectTrumpButton, okButton,
                                                        autoSelectTrumpButton, autoSelectPartnerButton,
                                                        autoSelectNestButton})
        button->rescale();
}

void MiddleDialog::reject()
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
