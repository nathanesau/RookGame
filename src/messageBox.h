#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include "clickableCard.h"
#include "common.h"
#include "scaledWidgets.h"

const QSize MESSAGE_BOX_SIZE = {400, 250};

class MessageBox : public QDialogWithClickableCardArray
{
    ScaledQLabel *msgLabel;
    ScaledQPushButton *okButton;
    ClickableCardArray *messageBoxCards;

public:
    MessageBox(QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    void changeCardArrayDrawPosition(int newDrawPosition);

    void setText(const QString &text);
    void showCards(const CardVector &cardArr);
    void turnOnHyperLinks();

protected:
    void okButtonPressed();
    void hyperLinkClicked(const QString &link);

    void onCardClicked(ClickableCard *clickableCard);
};

#endif