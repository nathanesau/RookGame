/********************************************************************************
** Form generated from reading UI file 'middleDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIDDLEDIALOG_H
#define UI_MIDDLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include "scaledWidgets.h"

QT_BEGIN_NAMESPACE

class Ui_MiddleDialog
{
public:
    ScaledQLabel *trumpCategoryLabel;
    ScaledQPushButton *selectPartnerButton;
    ScaledQLabel *partnerCategoryLabel;
    ScaledQPushButton *selectTrumpButton;
    ScaledQLabel *trumpLabel;
    ScaledQPushButton *okButton;
    ScaledQPushButton *autoSelectTrumpButton;
    ScaledQPushButton *autoSelectPartnerButton;
    ScaledQLabel *nestCategoryLabel;
    ScaledQPushButton *selectNestButton;
    ScaledQPushButton *autoSelectNestButton;

    void setupUi(QDialog *MiddleDialog)
    {
        if (MiddleDialog->objectName().isEmpty())
            MiddleDialog->setObjectName(QStringLiteral("MiddleDialog"));
        MiddleDialog->resize(724, 435);
        trumpCategoryLabel = new ScaledQLabel(MiddleDialog);
        trumpCategoryLabel->setObjectName(QStringLiteral("trumpCategoryLabel"));
        trumpCategoryLabel->setGeometry(QRect(90, 160, 121, 31));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        trumpCategoryLabel->setFont(font);
        selectPartnerButton = new ScaledQPushButton(MiddleDialog);
        selectPartnerButton->setObjectName(QStringLiteral("selectPartnerButton"));
        selectPartnerButton->setGeometry(QRect(260, 270, 141, 28));
        QFont font1;
        font1.setPointSize(10);
        selectPartnerButton->setFont(font1);
        partnerCategoryLabel = new ScaledQLabel(MiddleDialog);
        partnerCategoryLabel->setObjectName(QStringLiteral("partnerCategoryLabel"));
        partnerCategoryLabel->setGeometry(QRect(90, 270, 131, 31));
        partnerCategoryLabel->setFont(font);
        selectTrumpButton = new ScaledQPushButton(MiddleDialog);
        selectTrumpButton->setObjectName(QStringLiteral("selectTrumpButton"));
        selectTrumpButton->setGeometry(QRect(260, 160, 141, 31));
        selectTrumpButton->setFont(font1);
        trumpLabel = new ScaledQLabel(MiddleDialog);
        trumpLabel->setObjectName(QStringLiteral("trumpLabel"));
        trumpLabel->setGeometry(QRect(510, 150, 121, 51));
        trumpLabel->setAlignment(Qt::AlignCenter);
        trumpLabel->setFont(font1);
        okButton = new ScaledQPushButton(MiddleDialog);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setGeometry(QRect(620, 390, 75, 23));
        okButton->setFont(font1);
        autoSelectTrumpButton = new ScaledQPushButton(MiddleDialog);
        autoSelectTrumpButton->setObjectName(QStringLiteral("autoSelectTrumpButton"));
        autoSelectTrumpButton->setGeometry(QRect(260, 190, 141, 31));
        autoSelectTrumpButton->setFont(font1);
        autoSelectPartnerButton = new ScaledQPushButton(MiddleDialog);
        autoSelectPartnerButton->setObjectName(QStringLiteral("autoSelectPartnerButton"));
        autoSelectPartnerButton->setGeometry(QRect(260, 300, 141, 28));
        autoSelectPartnerButton->setFont(font1);
        nestCategoryLabel = new ScaledQLabel(MiddleDialog);
        nestCategoryLabel->setObjectName(QStringLiteral("nestCategoryLabel"));
        nestCategoryLabel->setGeometry(QRect(90, 40, 121, 31));
        nestCategoryLabel->setFont(font);
        selectNestButton = new ScaledQPushButton(MiddleDialog);
        selectNestButton->setObjectName(QStringLiteral("selectNestButton"));
        selectNestButton->setGeometry(QRect(260, 40, 141, 31));
        selectNestButton->setFont(font1);
        autoSelectNestButton = new ScaledQPushButton(MiddleDialog);
        autoSelectNestButton->setObjectName(QStringLiteral("autoSelectNestButton"));
        autoSelectNestButton->setGeometry(QRect(260, 70, 141, 31));
        autoSelectNestButton->setFont(font1);

        retranslateUi(MiddleDialog);

        QMetaObject::connectSlotsByName(MiddleDialog);
    } // setupUi

    void retranslateUi(QDialog *MiddleDialog)
    {
        MiddleDialog->setWindowTitle(QApplication::translate("MiddleDialog", "Middle Dialog", 0));
        trumpCategoryLabel->setText(QApplication::translate("MiddleDialog", "Trump", 0));
        selectPartnerButton->setText(QApplication::translate("MiddleDialog", "Select partner...", 0));
        partnerCategoryLabel->setText(QApplication::translate("MiddleDialog", "Partner", 0));
        selectTrumpButton->setText(QApplication::translate("MiddleDialog", "Select trump...", 0));
        trumpLabel->setText(QApplication::translate("MiddleDialog", "No Trump Selected", 0));
        okButton->setText(QApplication::translate("MiddleDialog", "OK", 0));
        autoSelectTrumpButton->setText(QApplication::translate("MiddleDialog", "Auto select trump...", 0));
        autoSelectPartnerButton->setText(QApplication::translate("MiddleDialog", "Auto select partner...", 0));
        nestCategoryLabel->setText(QApplication::translate("MiddleDialog", "Nest", 0));
        selectNestButton->setText(QApplication::translate("MiddleDialog", "Select nest...", 0));
        autoSelectNestButton->setText(QApplication::translate("MiddleDialog", "Auto select nest...", 0));
    } // retranslateUi

};

namespace Ui {
    class MiddleDialog: public Ui_MiddleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIDDLEDIALOG_H
