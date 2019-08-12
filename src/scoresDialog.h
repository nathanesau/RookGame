#ifndef SCORESDIALOG_H
#define SCORESDIALOG_H

#include <map>
#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>

#include "common.h"

using namespace std;

// forward declarations
class GameController;

// global declarations
extern GameController gc;

const QSize SCORES_DIALOG_SIZE = {320, 200};

// extend QDialog not ScaledQDialog
class ScoresDialog : public QDialog
{
    QTableWidget *tableWidget;

public:
    ScoresDialog(QWidget *parent = nullptr);

    void setupTableWidget(map<int, map<int, int>> &scoreHistory);
};

#endif