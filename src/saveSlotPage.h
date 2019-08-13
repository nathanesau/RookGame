#ifndef SAVESLOTPAGE_H
#define SAVESLOTPAGE_H

#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include "common.h"
#include "gameData.h"

// forward declarations
class GameDatabase;

// global declarations
extern GameDatabase db;

class SaveSlotPage : public QWidget
{
    GameData pageData;

    QGroupBox *existsGroup;
    QHBoxLayout *existsLayout;
    QLabel *existsLabel;

    QGroupBox *scoresGroup; // overall info
    QVBoxLayout *scoresLayout;
    QLabel *score1Label;
    QLabel *score2Label;
    QLabel *score3Label;
    QLabel *score4Label;

    QGroupBox *roundGroup; // current round info
    QVBoxLayout *roundLayout;
    QLabel *trumpLabel;
    QLabel *bidLabel;
    QLabel *partnerLabel;

    QGroupBox *handGroup; // current hand info
    QVBoxLayout *handLayout;
    QLabel *player1CardLabel;
    QLabel *player2CardLabel;
    QLabel *player3CardLabel;
    QLabel *player4CardLabel;

    QVBoxLayout *mainLayout;

public:
    SaveSlotPage(QString dbName, QWidget *parent = nullptr);
};

#endif