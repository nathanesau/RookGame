#include <QApplication>
#include <QCoreApplication>

#include "common.h"
#include "gameController.h"
#include "gameDatabase.h"
#include "cpuPlayer.h"
#include "mainWindow.h"
#include "utils.h"

// globals
CpuPlayer cpu;
GameController gc;
GameDatabase db;
QPixmapCache pixmapcache;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
    Utils::Ui::moveWindowToCenter(&mainWindow, -36);
    mainWindow.show();

    return a.exec();
}