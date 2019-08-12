#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QSize>
#include <QString>

#include "clickableCard.h"
#include "scaledWidgets.h"

const QSize DEFAULT_MSG_BOX_SIZE = {400, 250};

class MessageBox : public QDialogWithClickableCardArray
{
    ScaledQLabel *msgLabel;
    ScaledQPushButton *okButton;
    ClickableCardArray *messageBoxCards;

public:
    MessageBox(QWidget *parent = nullptr);
    virtual void rescale();
    virtual void reject();

    void setText(const QString &text);
    void showCards(const CardVector &cardArr);
    void turnOnHyperLinks();

protected:
    void okButtonPressed();
    void hyperLinkClicked(const QString &link);

    void onCardClicked(ClickableCard *clickableCard);
    void onCardHoverEnter(ClickableCard *clickableCard);
    void onCardHoverLeave(ClickableCard *clickableCard);
};

#endif