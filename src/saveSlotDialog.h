#ifndef SAVEGAMEDIALOG_H
#define SAVEGAMEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>

#include "saveSlotPage.h"

// DO NOT MODIFY
const int SLOT_UNDEFINED = -1;
const int SLOT_1 = 0;
const int SLOT_2 = 1;
const int SLOT_3 = 2;

// derive from QDialog not ScaledQDialog
class SaveSlotDialog : public QDialog
{
    int &slotSelected; // output of dialog

private:
    QListWidget *contentsWidget;
    QListWidgetItem *saveSlot1Button;
    QListWidgetItem *saveSlot2Button;
    QListWidgetItem *saveSlot3Button;

    QStackedWidget *pagesWidget;
    SaveSlotPage *saveSlot1Page;
    SaveSlotPage *saveSlot2Page;
    SaveSlotPage *saveSlot3Page;

    QHBoxLayout *horizontalLayout;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonsLayout;

    QPushButton *selectButton;

public:
    SaveSlotDialog(int &pSlotSelected, QWidget *parent = nullptr);    

    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void onSelectButton();
};

#endif