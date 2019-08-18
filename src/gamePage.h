#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "common.h"

class GamePage : public QWidget
{
    QGroupBox *bidGroup;
    QHBoxLayout *bidLayout;
    QLabel *cpuBidAggressionLabel;
    QComboBox *cpuBidAggressionBox;

    QGroupBox *nestGroup;
    QHBoxLayout *nestLayout;
    QLabel *numMiddleCardsAllowedLabel;
    QSpinBox *numMiddleCardsAllowedSpinBox;

    QPushButton *applyButton;

    QVBoxLayout *mainLayout;

    void initializeBidGroup();
    void initializeNestGroup();

public:
    GamePage(QWidget *parent = nullptr);

private:

    void onApply();

    void applyBid();
    void applyNest();
};

#endif