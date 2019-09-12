#ifndef CLICKABLECARD_H
#define CLICKABLECARD_H

#include "card.h"
#include "common.h"
#include "scaledWidgets.h"

// when adding a new draw position, review the following functions:
//      ClickableCard::getRotation
//      ClickableCardArray::performDynamicPositioning
//      ClickableCardArray::getWindowDimensions
//      ClickableCardArray::getVerticalShift
//      ClickableCardArray::getHorizontalShift
//      ClickableCardArray::getCardGap

const int DRAW_POSITION_UNDEFINED = -1;
const int DRAW_POSITION_MAIN_WIDGET_BOTTOM = 0;
const int DRAW_POSITION_MAIN_WIDGET_CENTER = 1;
const int DRAW_POSITION_MAIN_WIDGET_CENTER_BOTTOM = 2;
const int DRAW_POSITION_MAIN_WIDGET_CENTER_LEFT = 3;
const int DRAW_POSITION_MAIN_WIDGET_CENTER_TOP = 4;
const int DRAW_POSITION_MAIN_WIDGET_CENTER_RIGHT = 5;
const int DRAW_POSITION_MIDDLE_DLG_NEST = 6;
const int DRAW_POSITION_MIDDLE_DLG_PARTNER = 7;
const int DRAW_POSITION_NEST_DLG_TOP = 8;
const int DRAW_POSITION_NEST_DLG_BOTTOM = 9;
const int DRAW_POSITION_PARTNER_DLG = 10;
const int DRAW_POSITION_GAME_INFO_WIDGET = 11;
const int DRAW_POSITION_MESSAGE_BOX = 12;
const int DRAW_POSITION_MESSAGE_BOX_NEST = 13;

#ifdef CPU_DEBUG
const int DRAW_POSITION_MAIN_WIDGET_LEFT = 100;
const int DRAW_POSITION_MAIN_WIDGET_TOP = 101;
const int DRAW_POSITION_MAIN_WIDGET_RIGHT = 102;
#endif

const QSize SIZE_UNDEFINED = {0, 0};
const QSize SIZE_NORMAL = {180, 180};
const QSize SIZE_SMALL = {135, 135};
const QSize SIZE_TINY = {90, 90};

// forward declaration
struct CardKey;
struct CardPixmapKey;
struct CompareCardKey;
struct CompareCardPixmapKey;
class QDialogWithClickableCardArray;

// typedef declarations
typedef std::map<CardKey, std::string, CompareCardKey> CardStyleMap; 
typedef std::map<CardPixmapKey, std::unique_ptr<QPixmap>, CompareCardPixmapKey> QPixmapCache;

// global declarations
extern QPixmapCache pixmapcache;

class ClickableCard : public ScaledQLabel
{
    Q_OBJECT

public:
    Card data;

    ClickableCard(QDialogWithClickableCardArray *parent = nullptr);
    ClickableCard(const ClickableCard &pCard);
    ClickableCard &operator=(const ClickableCard &pCard);

    // Only difference from QLabel class is setData method
    // and override mousePressEvent
    void setData(const Card &pData, int drawPosition, QSize size, std::string style);

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    int getRotation(int drawPosition) const;
};

struct CardKey
{
    Card data;

    CardKey(Card pData);
};

struct CompareCardKey
{
    bool operator()(const CardKey& a, const CardKey& b) const;
};

struct CardPixmapKey
{
    Card data;
    int width;
    int height;
    int rotation;

    CardPixmapKey(Card pData, int pWidth, int pHeight, int pRotation);
};

struct CompareCardPixmapKey
{
    bool operator()(const CardPixmapKey& a, const CardPixmapKey& b) const;
};

class ClickableCardArray : public QWidget
{
    QDialogWithClickableCardArray *parent;

    std::vector<ClickableCard> clickableCards;

    int drawPosition;
    QSize size;

public:
    ClickableCardArray(int pDrawPosition, QSize pSize = SIZE_NORMAL, QDialogWithClickableCardArray *parent = nullptr);
    void rescale();

    void changeDrawPosition(int newDrawPosition);

    void showCards(const CardVector &cardArr, CardStyleMap *cardStyleMap = nullptr);
    void hideCards();

private:
    QPoint getCardPosition(int i, int n) const;

    bool useDynamicPositioning() const;

    // for dynamic positioning
    QSize getWindowDimensions() const;
    int getVerticalShift() const;
    int getHorizontalShift() const;
    int getCardGap() const;
};

class QDialogWithClickableCardArray : public ScaledQDialog
{
public:
    QDialogWithClickableCardArray(bool pFixedSize, QWidget *parent = nullptr);
    virtual void rescale() = 0;

    virtual void onCardClicked(ClickableCard *clickableCard) = 0;
};

#endif