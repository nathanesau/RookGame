#include "card.h"
#include "scaledWidgets.h"
#include "settings.h"
#include "utils.h"

ScaledQLabel::ScaledQLabel(QWidget *parent) : QLabel(parent)
{
    scaleFactor = Settings::Appearance::readScaleFactor();
    prevScaleFactor = scaleFactor;
}

void ScaledQLabel::rescale()
{
    updateScaleFactor();
    setFont(font());
    setGeometry(geometry());
}

void ScaledQLabel::setFont(const QFont &font)
{
    auto scaledFont = font;
    scaledFont.setPointSizeF(scaledFont.pointSizeF() * scaleFactor);

    QLabel::setFont(scaledFont);
}

void ScaledQLabel::setGeometry(const QRect &rect)
{
    auto scaledRect = QRect(rect.x() * scaleFactor, rect.y() * scaleFactor,
                            rect.width() * scaleFactor, rect.height() * scaleFactor);

    QLabel::setGeometry(scaledRect);
}

void ScaledQLabel::resize(int w, int h)
{
    QLabel::resize(w * scaleFactor, h * scaleFactor);
}

void ScaledQLabel::move(const QPoint &pos)
{
    QLabel::move(pos * scaleFactor);
}

void ScaledQLabel::updateScaleFactor()
{
    float newScaleFactor = Settings::Appearance::readScaleFactor();
    scaleFactor = newScaleFactor / prevScaleFactor;
    prevScaleFactor = newScaleFactor;
}

ScaledQPushButton::ScaledQPushButton(QWidget *parent) : QPushButton(parent)
{
    scaleFactor = Settings::Appearance::readScaleFactor();
    prevScaleFactor = scaleFactor;
}

void ScaledQPushButton::rescale()
{
    updateScaleFactor();
    setFont(font());
    setGeometry(geometry());
}

void ScaledQPushButton::setFont(const QFont &font)
{
    auto scaledFont = font;
    scaledFont.setPointSizeF(font.pointSizeF() * scaleFactor);

    QPushButton::setFont(scaledFont);
}

void ScaledQPushButton::setGeometry(const QRect &rect)
{
    auto scaledRect = QRect(rect.x() * scaleFactor, rect.y() * scaleFactor,
                            rect.width() * scaleFactor, rect.height() * scaleFactor);

    QPushButton::setGeometry(scaledRect);
}

void ScaledQPushButton::move(const QPoint &pos)
{
    QPushButton::move(pos * scaleFactor);
}

void ScaledQPushButton::resize(int w, int h)
{
    QPushButton::resize(w * scaleFactor, h * scaleFactor);
}

void ScaledQPushButton::updateScaleFactor()
{
    float newScaleFactor = Settings::Appearance::readScaleFactor();
    scaleFactor = newScaleFactor / prevScaleFactor;
    prevScaleFactor = newScaleFactor;
}

ScaledQComboBox::ScaledQComboBox(QWidget *parent) : QComboBox(parent)
{
    scaleFactor = Settings::Appearance::readScaleFactor();
    prevScaleFactor = scaleFactor;
}

void ScaledQComboBox::rescale()
{
    updateScaleFactor();
    setFont(font());
    setGeometry(geometry());
}

void ScaledQComboBox::setFont(const QFont &font)
{
    auto scaledFont = font;
    scaledFont.setPointSizeF(font.pointSizeF() * scaleFactor);

    QComboBox::setFont(scaledFont);
}

void ScaledQComboBox::setGeometry(const QRect &rect)
{
    auto scaledRect = QRect(rect.x() * scaleFactor, rect.y() * scaleFactor,
                            rect.width() * scaleFactor, rect.height() * scaleFactor);

    QComboBox::setGeometry(scaledRect);
}

void ScaledQComboBox::resize(int w, int h)
{
    QComboBox::resize(w * scaleFactor, h * scaleFactor);
}

void ScaledQComboBox::move(const QPoint &pos)
{
    QComboBox::move(pos * scaleFactor);
}

void ScaledQComboBox::updateScaleFactor()
{
    float newScaleFactor = Settings::Appearance::readScaleFactor();
    scaleFactor = newScaleFactor / prevScaleFactor;
    prevScaleFactor = newScaleFactor;
}

ScaledQCheckBox::ScaledQCheckBox(QWidget *parent) : QCheckBox(parent)
{
    scaleFactor = Settings::Appearance::readScaleFactor();
    prevScaleFactor = scaleFactor;
}

void ScaledQCheckBox::rescale()
{
    updateScaleFactor();
    setFont(font());
    setGeometry(geometry());
}

void ScaledQCheckBox::setFont(const QFont &font)
{
    auto scaledFont = font;
    scaledFont.setPointSizeF(font.pointSizeF() * scaleFactor);

    QCheckBox::setFont(scaledFont);
}

void ScaledQCheckBox::setGeometry(const QRect &rect)
{
    auto scaledRect = QRect(rect.x() * scaleFactor, rect.y() * scaleFactor,
                            rect.width() * scaleFactor, rect.height() * scaleFactor);

    QCheckBox::setGeometry(scaledRect);
}

void ScaledQCheckBox::resize(int w, int h)
{
    QCheckBox::resize(w * scaleFactor, h * scaleFactor);
}

void ScaledQCheckBox::move(const QPoint &pos)
{
    QCheckBox::move(pos * scaleFactor);
}

void ScaledQCheckBox::updateScaleFactor()
{
    float newScaleFactor = Settings::Appearance::readScaleFactor();
    scaleFactor = newScaleFactor / prevScaleFactor;
    prevScaleFactor = newScaleFactor;
}

ScaledQProgressBar::ScaledQProgressBar(QWidget *parent) : QProgressBar(parent)
{
    scaleFactor = Settings::Appearance::readScaleFactor();
    prevScaleFactor = scaleFactor;
}

void ScaledQProgressBar::rescale()
{
    updateScaleFactor();
    setFont(font());
    setGeometry(geometry());
}

void ScaledQProgressBar::setFont(const QFont &font)
{
    auto scaledFont = font;
    scaledFont.setPointSizeF(font.pointSizeF() * scaleFactor);

    QProgressBar::setFont(scaledFont);
}

void ScaledQProgressBar::setGeometry(const QRect &rect)
{
    auto scaledRect = QRect(rect.x() * scaleFactor, rect.y() * scaleFactor,
                            rect.width() * scaleFactor, rect.height() * scaleFactor);

    QProgressBar::setGeometry(scaledRect);
}

void ScaledQProgressBar::resize(int w, int h)
{
    QProgressBar::resize(w * scaleFactor, h * scaleFactor);
}

void ScaledQProgressBar::move(const QPoint &pos)
{
    QProgressBar::move(pos * scaleFactor);
}

void ScaledQProgressBar::updateScaleFactor()
{
    float newScaleFactor = Settings::Appearance::readScaleFactor();
    scaleFactor = newScaleFactor / prevScaleFactor;
    prevScaleFactor = newScaleFactor;
}

ScaledQDialog::ScaledQDialog(bool pFixedSize, QWidget *parent) : QDialog(parent)
{
    fixedSize = pFixedSize;
    scaleFactor = Settings::Appearance::readScaleFactor();
    prevScaleFactor = scaleFactor;
}

void ScaledQDialog::setGeometry(const QRect &rect)
{
    auto scaledRect = QRect(rect.x() * scaleFactor, rect.y() * scaleFactor,
                            rect.width() * scaleFactor, rect.height() * scaleFactor);

    if (fixedSize)
    {
        QDialog::setFixedSize(scaledRect.width(), scaledRect.height());
        QDialog::setMaximumSize(scaledRect.width(), scaledRect.height());
    }

    QDialog::setGeometry(scaledRect);
}

void ScaledQDialog::resize(int w, int h)
{
    if (fixedSize)
    {
        QDialog::setFixedSize(w * scaleFactor, h * scaleFactor);
        QDialog::setMaximumSize(w * scaleFactor, h * scaleFactor);
    }

    QDialog::resize(w * scaleFactor, h * scaleFactor);
}

void ScaledQDialog::resize(const QSize &pSize)
{
    if (fixedSize)
    {
        QDialog::setFixedSize(pSize * scaleFactor);
        QDialog::setMaximumSize(pSize * scaleFactor);
    }
    
    QDialog::resize(pSize * scaleFactor);
}

void ScaledQDialog::move(const QPoint &pos)
{
    QDialog::move(pos * scaleFactor);
}

void ScaledQDialog::updateScaleFactor()
{
    float newScaleFactor = Settings::Appearance::readScaleFactor();
    scaleFactor = newScaleFactor / prevScaleFactor;
    prevScaleFactor = newScaleFactor;
}

ScaledQMainWindow::ScaledQMainWindow(QWidget *parent) : QMainWindow(parent)
{
    scaleFactor = Settings::Appearance::readScaleFactor();
    prevScaleFactor = scaleFactor;
}

void ScaledQMainWindow::setGeometry(const QRect &rect)
{
    auto scaledRect = QRect(rect.x() * scaleFactor, rect.y() * scaleFactor,
                            rect.width() * scaleFactor, rect.height() * scaleFactor);

    QMainWindow::setFixedSize(scaledRect.width(), scaledRect.height());
    QMainWindow::setMaximumSize(scaledRect.width(), scaledRect.height());
    QMainWindow::setGeometry(scaledRect);
}

void ScaledQMainWindow::resize(int w, int h)
{
    QMainWindow::setFixedSize(w * scaleFactor, h * scaleFactor);
    QMainWindow::setMaximumSize(w * scaleFactor, h * scaleFactor);
    QMainWindow::resize(w * scaleFactor, h * scaleFactor);
}

void ScaledQMainWindow::resize(const QSize &pSize)
{
    QMainWindow::setFixedSize(pSize * scaleFactor);
    QMainWindow::setMaximumSize(pSize * scaleFactor);
    QMainWindow::resize(pSize * scaleFactor);
}

void ScaledQMainWindow::move(const QPoint &pos)
{
    QMainWindow::move(pos * scaleFactor);
}

void ScaledQMainWindow::updateScaleFactor()
{
    float newScaleFactor = Settings::Appearance::readScaleFactor();
    scaleFactor = newScaleFactor / prevScaleFactor;
    prevScaleFactor = newScaleFactor;
}