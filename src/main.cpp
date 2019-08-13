#include <QApplication>
#include <QCoreApplication>

#include "common.h"
#include "gameData.h"
#include "gameDatabase.h"
#include "cpuPlayer.h"
#include "mainWindow.h"
#include "utils.h"

// globals
CpuPlayer cpu;
GameData gamedata;
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