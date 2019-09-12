#ifndef GAMEPAGE_H
#define GAMEPAGE_H

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

    QGroupBox *partnerGroup;
    QVBoxLayout *partnerLayout;
    QCheckBox *pickSelfAsPartnerCheckBox;
    QCheckBox *pickNestAsPartnerCheckBox;

    QGroupBox *cpuGroup;
    QHBoxLayout *cpuLayout;
    QLabel *cpuRandomnessLabel;
    QComboBox *cpuRandomnessBox;

    QPushButton *applyButton;

    QVBoxLayout *mainLayout;

    void initializeBidGroup();
    void initializeNestGroup();
    void initializePartnerGroup();
    void initializeCpuGroup();

public:
    GamePage(QWidget *parent = nullptr);

private:

    void onApply();

    void applyBidGroup();
    void applyNestGroup();
    void applyPartnerGroup();
    void applyCpuGroup();
};

#endif