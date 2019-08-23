#ifndef UTILS_H
#define UTILS_H

#include <QDialog>
#include <QMainWindow>
#include <QWidget>

#include "common.h"

using namespace std;

// constants
const int DIALOG_POSITION_CENTER = 0;
const int DIALOG_POSITION_MIDDLE_DLG = 1;
const int DIALOG_POSITION_NEST_DLG = 2;
const int DIALOG_POSITION_TRUMP_DLG = 3;
const int DIALOG_POSITION_PARTNER_DLG = 4;
const int DIALOG_POSITION_CARD_MESSAGE_BOX = 5;

namespace Utils
{
namespace Ui
{
// get size of main monitor
QRect getScreenGeometry();

// positioning UI elements
void moveWindowToCenter(QWidget *widget, int verticalShift = 0);
void moveParentlessDialog(QDialog *dialog, QMainWindow *mainWindow, int position);

} // namespace Ui

namespace Stat
{
// standard normal distribution cdf
double phi(double x);
} // namespace Utils

namespace Random
{
int randbetween(int a, int b);
}
}

#endif