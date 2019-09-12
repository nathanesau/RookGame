#ifndef APPEARANCEPAGE_H
#define APPEARANCEPAGE_H

#include "common.h"

const float MIN_SCALE_FACTOR = 0.6F;
const float SCALE_FACTOR_INCR = 0.05F;

// windowed app will not take up for of screen than this
const float MAX_PROPORTION_SCREEN = 0.975F;

// forward declarations
class MainWindow;

class AppearancePage : public QWidget
{
    MainWindow *mainWindow; // non-owning

    std::map<int, float> indexScaleFactorMap;

    QGroupBox *resolutionGroup;
    QHBoxLayout *resolutionLayout;
    QLabel *resolutionLabel;
    QComboBox *resolutionComboBox;

    QGroupBox *namesGroup;
    QVBoxLayout *namesLayout;
    QLabel *player1NameLabel;
    QLineEdit *player1NameEdit;
    QLabel *player2NameLabel;
    QLineEdit *player2NameEdit;
    QLabel *player3NameLabel;
    QLineEdit *player3NameEdit;
    QLabel *player4NameLabel;
    QLineEdit *player4NameEdit;

    QGroupBox *hudGroup;
    QVBoxLayout *hudLayout;
    QCheckBox *showNameTagsBox;
    QCheckBox *showPartnerToolTipBox;

    QGroupBox *screenGroup;
    QVBoxLayout *screenLayout;
    QLineEdit *screenWidthLineEdit;
    QLineEdit *screenHeightLineEdit;

    QPushButton *applyButton;
    
    QVBoxLayout *mainLayout;

private:
    void initializeResolutionGroup();
    void initializeNamesGroup();
    void initializeHUDGroup();
    void initializeScreenGroup();
public:

    AppearancePage(MainWindow *pMainWindow, QWidget *parent = 0);

private:
    void onApply();

    void applyResolution();
    void applyPlayerNames();
    void applyHUD();
    void applyScreenDimensions();
};

#endif