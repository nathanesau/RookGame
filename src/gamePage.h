#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class GamePage : public QWidget
{
    QGroupBox *nestGroup;
    QHBoxLayout *nestLayout;
    QLabel *numMiddleCardsAllowedLabel;
    QSpinBox *numMiddleCardsAllowedSpinBox;

    QPushButton *applyButton;

    QVBoxLayout *mainLayout;

public:
    GamePage(QWidget *parent = nullptr);

private:
    void onApply();

    void applyNest();
};

#endif