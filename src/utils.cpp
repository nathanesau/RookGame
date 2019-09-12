#include "settings.h"
#include "utils.h"

namespace Utils
{
namespace Ui
{
QRect getScreenGeometry()
{
    QRect geometry = QApplication::desktop()->screenGeometry();
    geometry.setWidth(Settings::Appearance::readScreenWidth());
    geometry.setHeight(Settings::Appearance::readScreenHeight());

    return geometry;
}

void moveWindowToCenter(QWidget *widget, int verticalShift)
{
    float scaleFactor = Settings::Appearance::readScaleFactor();
    auto screenGeometry = Utils::Ui::getScreenGeometry();
    int x = (screenGeometry.width() - widget->width()) / 2;
    int y = std::max(0, (screenGeometry.height() - widget->height()) / 2) + verticalShift * scaleFactor;
    widget->move(QPoint(x, y));
}

void moveParentlessDialog(QDialog *dialog, QMainWindow *mainWindow, int position)
{
    assert(!dialog->parent());

    auto mwPos = mainWindow->pos();
    auto mwSize = mainWindow->size();
    auto mwCenter = QPoint(mwPos.x() + mwSize.width() / 2, mwPos.y() + mwSize.height() / 2);
    auto dSize = dialog->size();

    auto dPos = [&]() -> QPoint {
        auto centerPos = QPoint(mwCenter.x() - dSize.width() / 2, mwCenter.y() - dSize.height() / 2);

        switch (position)
        {
        case DIALOG_POSITION_CENTER:
            return centerPos;

        case DIALOG_POSITION_MIDDLE_DLG:
        case DIALOG_POSITION_NEST_DLG:
        case DIALOG_POSITION_TRUMP_DLG:
        case DIALOG_POSITION_PARTNER_DLG:
            return QPoint(centerPos.x(), centerPos.y() - mwSize.height() * 0.1F);

        case DIALOG_POSITION_CARD_MESSAGE_BOX:
            return QPoint(centerPos.x() + mwSize.width() * 0.35F, centerPos.y());

        default:
            return {0, 0};
        }
    }();

    dialog->move(QPoint({std::max(0, dPos.x()), std::max(0, dPos.y())}));
}
} // namespace Ui

namespace Stat
{
double phi(double x)
{
    // constants
    double a1 = 0.254829592;
    double a2 = -0.284496736;
    double a3 = 1.421413741;
    double a4 = -1.453152027;
    double a5 = 1.061405429;
    double p = 0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x) / sqrt(2.0);

    // A&S formula 7.1.26
    double t = 1.0 / (1.0 + p * x);
    double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * exp(-x * x);

    return 0.5 * (1.0 + sign * y);
}
} // namespace Stat

namespace Random
{
int randbetween(int a, int b)
{
    // rand() % (b - a + 1):        random number between 0 and (b - a)
    // rand() & (b - a + 1) + a:    random number between a and b
    return std::rand() % (b - a + 1) + a;
}
} // namespace Random

} // namespace Utils
