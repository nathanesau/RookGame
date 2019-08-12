#ifndef SCORESDIALOG_H
#define SCORESDIALOG_H

#include <map>
#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>

using namespace std;

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