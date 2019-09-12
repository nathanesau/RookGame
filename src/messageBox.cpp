#include "messageBox.h"

MessageBox::MessageBox(QWidget *parent) : QDialogWithClickableCardArray(true, parent)
{
    msgLabel = new ScaledQLabel(this);
    msgLabel->setAlignment(Qt::AlignTop);
    msgLabel->setGeometry(QRect(20, 20, 400, 200));
    msgLabel->setFont(QFont("Times", 10));

    okButton = new ScaledQPushButton(this);
    okButton->setGeometry(QRect(30, 210, 101, 28));
    okButton->setFont(QFont("Times", 10));
    okButton->setText("OK");

    QObject::connect(okButton, &QPushButton::pressed, this, &MessageBox::okButtonPressed);

    // by default, all message boxes use draw position DRAW_POSITION_MESSAGE_BOX
    // this can be overriden using method "setCardsDrawPosition"
    messageBoxCards = new ClickableCardArray(DRAW_POSITION_MESSAGE_BOX, SIZE_SMALL, this);
    messageBoxCards->hide();

    resize(MESSAGE_BOX_SIZE);
    setWindowIcon(QIcon(":rookicon.gif"));
    setStyleSheet("background-color: white");
    setWindowOpacity(1.0);
}

void MessageBox::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for(auto label : std::vector<ScaledQLabel *>{msgLabel})
        label->rescale();
    
    for(auto button : std::vector<ScaledQPushButton *>{okButton})
        button->rescale();

    for(auto clickableCardArray : std::vector<ClickableCardArray *>{messageBoxCards})
        clickableCardArray->rescale();
}

void MessageBox::reject()
{
    // do nothing
}

void MessageBox::changeCardArrayDrawPosition(int newDrawPosition)
{
    messageBoxCards->changeDrawPosition(newDrawPosition);
}

void MessageBox::setText(const QString &text)
{
    msgLabel->setText(text);
}

void MessageBox::showCards(const CardVector &cardArr)
{
    messageBoxCards->showCards(cardArr);
}

void MessageBox::turnOnHyperLinks()
{
    msgLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    QObject::connect(msgLabel, &QLabel::linkActivated, this, &MessageBox::hyperLinkClicked);
}

void MessageBox::okButtonPressed()
{
    QDialog::accept();
}

void MessageBox::hyperLinkClicked(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void MessageBox::onCardClicked(ClickableCard *clickableCard)
{
    // do nothing
}

void MessageBox::onCardHoverEnter(ClickableCard *clickableCard)
{
    // do nothing
}

void MessageBox::onCardHoverLeave(ClickableCard *clickableCard)
{
    // do nothing
}