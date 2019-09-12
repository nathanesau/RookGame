#include "common.h"
#include "gameData.h"
#include "gameDatabase.h"
#include "cpu.h"
#include "mainWindow.h"
#include "utils.h"

//#ifndef CPU_DEBUG
//#define CPU_DEBUG
//#endif

// globals
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