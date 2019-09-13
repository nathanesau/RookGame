#ifndef SCORESDIALOG_H
#define SCORESDIALOG_H

#include "common.h"

// forward declarations
struct GameData;

// global declarations
extern GameData gamedata;

const QSize SCORES_DIALOG_SIZE = {320, 200};

// extend QDialog not ScaledQDialog
class ScoresDialog : public QDialog
{
    QVBoxLayout *mainLayout;
    QTableWidget *tableWidget;

public:
    ScoresDialog(QWidget *parent = nullptr);

    void setupTableWidget(const std::map<int, std::map<int, int>> &scoreHistory);
};

#endif
