#ifndef SCALEDWIDGETS_H
#define SCALEDWIDGETS_H

#include "common.h"

// forward declarations
struct Card;
struct CardPixmapKey;
struct CompareCardPixmapKey;

// typedef declarations
typedef std::map<CardPixmapKey, std::unique_ptr<QPixmap>, CompareCardPixmapKey> QPixmapCache;

// global declarations
extern QPixmapCache pixmapcache;

class ScaledQLabel : public QLabel
{
    float prevScaleFactor;
    float scaleFactor;

public:
    ScaledQLabel(QWidget *parent = nullptr);
    void rescale();

    // override functions related to size or position
    void setFont(const QFont &font);
    void setGeometry(const QRect &rect);
    void resize(int w, int h);
    void move(const QPoint &pos);

protected:
    void updateScaleFactor();
};

class ScaledQPushButton : public QPushButton
{
    float prevScaleFactor;
    float scaleFactor;

public:
    ScaledQPushButton(QWidget *parent = nullptr);
    void rescale();

    // override functions related to size or position
    virtual void setFont(const QFont &font);
    virtual void setGeometry(const QRect &rect);
    virtual void resize(int w, int h);
    virtual void move(const QPoint &pos);

protected:
    void updateScaleFactor();
};

class ScaledQComboBox : public QComboBox
{
    float prevScaleFactor;
    float scaleFactor;

public:
    ScaledQComboBox(QWidget *parent);
    void rescale();

    // override functions related to size or position
    virtual void setFont(const QFont &font);
    virtual void setGeometry(const QRect &rect);
    virtual void resize(int w, int h);
    virtual void move(const QPoint &pos);

protected:
    void updateScaleFactor();
};

class ScaledQCheckBox : public QCheckBox
{
    float prevScaleFactor;
    float scaleFactor;

public:
    ScaledQCheckBox(QWidget *parent = nullptr);
    void rescale();

    // override functions related to size or position
    virtual void setFont(const QFont &font);
    virtual void setGeometry(const QRect &rect);
    virtual void resize(int w, int h);
    virtual void move(const QPoint &pos);

protected:
    void updateScaleFactor();
};

class ScaledQProgressBar : public QProgressBar
{
    float prevScaleFactor;
    float scaleFactor;

public:
    ScaledQProgressBar(QWidget *parent = nullptr);
    void rescale();

    // override functions related to size or position
    virtual void setFont(const QFont &font);
    virtual void setGeometry(const QRect &rect);
    virtual void resize(int w, int h);
    virtual void move(const QPoint &pos);

protected:
    void updateScaleFactor();
};

class ScaledQDialog : public QDialog
{
    bool fixedSize;
    float prevScaleFactor;
    float scaleFactor;

public:
    ScaledQDialog(bool pFixedSize, QWidget *parent = nullptr);

    // should call updateScaleFactor() and setGeometry(geometry())
    virtual void rescale() = 0;

    // override functions related to size or position
    virtual void setGeometry(const QRect &rect);
    virtual void resize(int w, int h);
    virtual void resize(const QSize &pSize);
    virtual void move(const QPoint &pos);

protected:
    void updateScaleFactor();
};

class ScaledQMainWindow : public QMainWindow
{
    float prevScaleFactor;
    float scaleFactor;

public:
    ScaledQMainWindow(QWidget *parent = nullptr);
    virtual void rescale() = 0;

    // override functions related to size or position
    virtual void setGeometry(const QRect &rect);
    virtual void resize(int w, int h);
    virtual void resize(const QSize &pSize);
    virtual void move(const QPoint &pos);

protected:
    void updateScaleFactor();
};

#endif