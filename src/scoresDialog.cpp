#include "gameData.h"
#include "scoresDialog.h"

ScoresDialog::ScoresDialog(QWidget *parent) : QDialog(parent)
{
    tableWidget = new QTableWidget;
    
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableWidget);
    
    resize(SCORES_DIALOG_SIZE);
    setLayout(mainLayout);
    setWindowIcon(QIcon(":rookicon.gif"));
    setWindowTitle("Past Round Scores");
}

void ScoresDialog::setupTableWidget(const std::map<int, std::map<int, int>> &scoreHistory)
{
    tableWidget->setStyleSheet("background-color: white;");
    tableWidget->setRowCount((int) scoreHistory.size() * 4);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels({"Round", "Player", "Score"});
    tableWidget->resize(size()); // fill entire dialog

    // table widget items will have tableWidget as parent
    int row = 0;
    for(auto &round : scoreHistory)
    {
        for(auto &player : round.second)
        {
            tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(round.first))); // roundNo
            tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(gamedata.playerArr[player.first].getPlayerName())));
            tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(player.second))); // score
            row++;
        }
    }

    tableWidget->show();
}
