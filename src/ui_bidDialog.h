/********************************************************************************
** Form generated from reading UI file 'bidDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BIDDIALOG_H
#define UI_BIDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include "scaledWidgets.h"

QT_BEGIN_NAMESPACE

class Ui_BidDialog
{
public:
    ScaledQLabel *player1Label;
    ScaledQLabel *player3Label;
    ScaledQLabel *player2Label;
    ScaledQLabel *player4Label;
    ScaledQLabel *player2BidLabel;
    ScaledQLabel *player3BidLabel;
    ScaledQLabel *player4BidLabel;
    ScaledQLabel *player1BidLabel;
    ScaledQPushButton *bidButton;
    ScaledQPushButton *passButton;
    ScaledQComboBox *bidAmountComboBox;
    ScaledQLabel *bidAmountLabel;

    void setupUi(QDialog *BidDialog)
    {
        if (BidDialog->objectName().isEmpty())
            BidDialog->setObjectName(QStringLiteral("BidDialog"));
        BidDialog->resize(661, 302);
        player1Label = new ScaledQLabel(BidDialog);
        player1Label->setObjectName(QStringLiteral("player1Label"));
        player1Label->setGeometry(QRect(320, 230, 81, 21));
        QFont font;
        font.setPointSize(12);
        player1Label->setFont(font);
        player3Label = new ScaledQLabel(BidDialog);
        player3Label->setObjectName(QStringLiteral("player3Label"));
        player3Label->setGeometry(QRect(310, 20, 81, 21));
        player3Label->setFont(font);
        player2Label = new ScaledQLabel(BidDialog);
        player2Label->setObjectName(QStringLiteral("player2Label"));
        player2Label->setGeometry(QRect(70, 110, 81, 21));
        player2Label->setFont(font);
        player4Label = new ScaledQLabel(BidDialog);
        player4Label->setObjectName(QStringLiteral("player4Label"));
        player4Label->setGeometry(QRect(540, 110, 71, 21));
        player4Label->setFont(font);
        player2BidLabel = new ScaledQLabel(BidDialog);
        player2BidLabel->setObjectName(QStringLiteral("player2BidLabel"));
        player2BidLabel->setGeometry(QRect(90, 140, 41, 16));
        player2BidLabel->setFont(font);
        player3BidLabel = new ScaledQLabel(BidDialog);
        player3BidLabel->setObjectName(QStringLiteral("player3BidLabel"));
        player3BidLabel->setGeometry(QRect(330, 50, 41, 20));
        player3BidLabel->setFont(font);
        player4BidLabel = new ScaledQLabel(BidDialog);
        player4BidLabel->setObjectName(QStringLiteral("player4BidLabel"));
        player4BidLabel->setGeometry(QRect(560, 140, 41, 20));
        player4BidLabel->setFont(font);
        player1BidLabel = new ScaledQLabel(BidDialog);
        player1BidLabel->setObjectName(QStringLiteral("player1BidLabel"));
        player1BidLabel->setGeometry(QRect(340, 260, 41, 20));
        player1BidLabel->setFont(font);
        bidButton = new ScaledQPushButton(BidDialog);
        bidButton->setObjectName(QStringLiteral("bidButton"));
        bidButton->setGeometry(QRect(160, 200, 71, 31));
        QFont font1;
        font1.setPointSize(10);
        bidButton->setFont(font1);
        passButton = new ScaledQPushButton(BidDialog);
        passButton->setObjectName(QStringLiteral("passButton"));
        passButton->setGeometry(QRect(440, 200, 81, 31));
        passButton->setFont(font1);
        bidAmountComboBox = new ScaledQComboBox(BidDialog);
        bidAmountComboBox->setObjectName(QStringLiteral("bidAmountComboBox"));
        bidAmountComboBox->setGeometry(QRect(280, 150, 141, 21));
        bidAmountComboBox->setFont(font);
        bidAmountLabel = new ScaledQLabel(BidDialog);
        bidAmountLabel->setObjectName(QStringLiteral("bidAmountLabel"));
        bidAmountLabel->setGeometry(QRect(300, 110, 121, 21));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        bidAmountLabel->setFont(font2);

        retranslateUi(BidDialog);

        QMetaObject::connectSlotsByName(BidDialog);
    } // setupUi

    void retranslateUi(QDialog *BidDialog)
    {
        BidDialog->setWindowTitle(QApplication::translate("BidDialog", "Bid Dialog", 0));
        player1Label->setProperty("text", QVariant(QString()));
        player3Label->setProperty("text", QVariant(QString()));
        player2Label->setProperty("text", QVariant(QString()));
        player4Label->setProperty("text", QVariant(QString()));
        player2BidLabel->setProperty("text", QVariant(QApplication::translate("BidDialog", "0", 0)));
        player3BidLabel->setProperty("text", QVariant(QApplication::translate("BidDialog", "0", 0)));
        player4BidLabel->setProperty("text", QVariant(QApplication::translate("BidDialog", "0", 0)));
        player1BidLabel->setProperty("text", QVariant(QApplication::translate("BidDialog", "0", 0)));
        bidButton->setProperty("text", QVariant(QApplication::translate("BidDialog", "Bid", 0)));
        passButton->setProperty("text", QVariant(QApplication::translate("BidDialog", "Pass", 0)));
        bidAmountLabel->setProperty("text", QVariant(QApplication::translate("BidDialog", "Bid Amount", 0)));
    } // retranslateUi

};

namespace Ui {
    class BidDialog: public Ui_BidDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIDDIALOG_H
