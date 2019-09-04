#include <array>
#include <QPushButton>
#include <QObject>
#include <string>

#include "gameData.h"
#include "mainWindow.h"
#include "messageBox.h"
#include "middleDialog.h"
#include "nestDialog.h"
#include "partnerDialog.h"
#include "trumpDialog.h"
#include "utils.h"

using namespace std;

MiddleDialog::MiddleDialog(MainWidget *pMainWidget, QMainWindow *pMainWindow, QWidget *parent) : mainWidget(pMainWidget),
                                                                                                 mainWindow(pMainWindow),
                                                                                                 QDialogWithClickableCardArray(true, parent),
                                                                                                 originalNest(gamedata.nest)
{
    ui.setupUi(this);

    nestCards = new ClickableCardArray(DRAW_POSITION_MIDDLE_DLG_NEST, SIZE_TINY, this);
    nestCards->showCards(gamedata.nest);

    partnerCards = new ClickableCardArray(DRAW_POSITION_MIDDLE_DLG_PARTNER, SIZE_TINY, this);
    partnerCards->showCards({Card(SUIT_UNDEFINED, VALUE_UNDEFINED)});

    QObject::connect(ui.selectNestButton, &QPushButton::pressed,
                     this, &MiddleDialog::selectNestButtonPressed);

    QObject::connect(ui.autoSelectNestButton, &QPushButton::pressed,
                     this, &MiddleDialog::autoSelectNestButtonPressed);

    QObject::connect(ui.selectTrumpButton, &QPushButton::pressed,
                     this, &MiddleDialog::selectTrumpButtonPressed);

    QObject::connect(ui.autoSelectTrumpButton, &QPushButton::pressed,
                     this, &MiddleDialog::autoSelectTrumpButtonPressed);

    QObject::connect(ui.selectPartnerButton, &QPushButton::pressed,
                     this, &MiddleDialog::selectPartnerButtonPressed);

    QObject::connect(ui.autoSelectPartnerButton, &QPushButton::pressed,
                     this, &MiddleDialog::autoSelectPartnerButtonPressed);

    QObject::connect(ui.okButton, &QPushButton::pressed,
                     this, &MiddleDialog::okButtonPressed);

    resize(MIDDLE_DIALOG_SIZE);
    setWindowIcon(QIcon(":rookicon.gif"));
    setStyleSheet("background-color: white");
}

void MiddleDialog::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto clickableCardArray : vector<ClickableCardArray *>{nestCards, partnerCards})
        clickableCardArray->rescale();

    for (auto label : vector<ScaledQLabel *>{ui.trumpCategoryLabel, ui.partnerCategoryLabel, ui.trumpLabel,
                                             ui.nestCategoryLabel})
        label->rescale();

    for (auto button : vector<ScaledQPushButton *>{ui.selectPartnerButton, ui.selectTrumpButton, ui.okButton,
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

void MiddleDialog::onCardHoverEnter(ClickableCard *clickableCard)
{
    // do nothing
}

void MiddleDialog::onCardHoverLeave(ClickableCard *clickableCard)
{
    // do nothing
}

void MiddleDialog::selectNestButtonPressed()
{
    NestDialog nestDlg(originalNest, mainWindow);
    Utils::Ui::moveParentlessDialog(&nestDlg, mainWindow, DIALOG_POSITION_NEST_DLG);

    if (!nestDlg.exec())
    {
        qFatal("Problem executing nest dialog");
        return;
    }

    nestCards->showCards(gamedata.nest);

    // refresh nest, player 1 cards
    mainWidget->refreshCardWidgets(gamedata);
}

void MiddleDialog::autoSelectNestButtonPressed()
{
    gamedata.playerArr[PLAYER_1].cpu->selectNest();
    
    nestCards->showCards(gamedata.nest);

    // refresh nest, player 1 cards
    mainWidget->refreshCardWidgets(gamedata);
}

void MiddleDialog::selectTrumpButtonPressed()
{
    TrumpDialog trumpDlg(gamedata.roundInfo.trump);
    Utils::Ui::moveParentlessDialog(&trumpDlg, mainWindow, DIALOG_POSITION_TRUMP_DLG);

    if (!trumpDlg.exec())
    {
        qFatal("Problem executing trump dialog");
        return;
    }

    setupTrumpLabel(gamedata.roundInfo.trump);
}

void MiddleDialog::autoSelectTrumpButtonPressed()
{
    gamedata.playerArr[PLAYER_1].cpu->selectTrump();

    setupTrumpLabel(gamedata.roundInfo.trump);
}

void MiddleDialog::selectPartnerButtonPressed()
{
    PartnerDialog partnerDlg(gamedata.roundInfo.partnerCard);
    Utils::Ui::moveParentlessDialog(&partnerDlg, mainWindow, DIALOG_POSITION_PARTNER_DLG);

    if (!partnerDlg.exec())
    {
        qFatal("Problem executing partner dialog");
        return;
    }

    partnerCards->showCards({gamedata.roundInfo.partnerCard});
}

void MiddleDialog::autoSelectPartnerButtonPressed()
{
    gamedata.playerArr[PLAYER_1].cpu->selectPartner();

    partnerCards->showCards({gamedata.roundInfo.partnerCard});
}

void MiddleDialog::okButtonPressed()
{
    if (gamedata.roundInfo.trump == SUIT_UNDEFINED || gamedata.roundInfo.partnerCard.isUndefined())
    {
        MessageBox msgBox;
        msgBox.setText("Trump and partner card must be selected");
        msgBox.setWindowTitle("Invalid selection");
        Utils::Ui::moveParentlessDialog(&msgBox, mainWindow, DIALOG_POSITION_CENTER);
        msgBox.exec();
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
