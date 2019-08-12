#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "AppearancePage.h"
#include "GamePage.h"

class PreferencesDialog : public QDialog
{
    MainWindow *mainWindow; // non-owning

    QHBoxLayout *horizontalLayout;

    QListWidget *contentsWidget;
    QListWidgetItem *appearanceButton;
    QListWidgetItem *gameButton;

    QStackedWidget *pagesWidget;
    AppearancePage *appearancePage;
    GamePage *gamePage;

    QHBoxLayout *buttonsLayout;
    QPushButton *closeButton;

    QVBoxLayout *mainLayout;

    void setupContentsWidget();
    void setupPagesWidget();

public:
    PreferencesDialog(MainWindow *pMainWindow, QWidget *parent = nullptr);

    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void onCloseButton();
};

#endif