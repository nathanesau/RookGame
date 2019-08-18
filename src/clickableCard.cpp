#include <QBrush>
#include <QPainter>

#include "clickableCard.h"
#include "mainWindow.h"
#include "messageBox.h"
#include "middleDialog.h"
#include "nestDialog.h"
#include "partnerDialog.h"
#include "settings.h"

ClickableCard::ClickableCard(QDialogWithClickableCardArray *parent) : ScaledQLabel(parent)
{
}

ClickableCard::ClickableCard(const ClickableCard &pCard) : data(pCard.data)
{
    // not set: parent, pixmap, stylesheet
}

ClickableCard &ClickableCard::operator=(const ClickableCard &pCard)
{
    // not set: parent, pixmap, stylesheet

    this->data = pCard.data;
    return *this;
}

void ClickableCard::setData(const Card &pData, int drawPosition, QSize size, string style)
{
    if (data != pData || data.isUndefined())
    {
        data = pData;

        float scaleFactor = Settings::Appearance::readScaleFactor();

        auto pixmapKey = CardPixmapKey(data,
                                       size.width() * scaleFactor,
                                       size.height() * scaleFactor,
                                       getRotation(drawPosition));

        auto &pixmap = pixmapcache[pixmapKey];

        if (!pixmap)
        {
            string fname = ":" + pixmapKey.data.getValueAsString() + pixmapKey.data.getSuitAsString() + ".gif";
            pixmap = make_unique<QPixmap>(QString::fromStdString(fname));

            *pixmap = pixmap->scaled(QSize(pixmapKey.width, pixmapKey.height), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QTransform transform;
            transform.rotate(pixmapKey.rotation);
            *pixmap = pixmap->transformed(transform);

            QLabel::setPixmap(*pixmap);
        }
        else
        {
            QLabel::setPixmap(*pixmap);
        }

        setStyleSheet(QString::fromStdString(style));
    }
}

void ClickableCard::mousePressEvent(QMouseEvent *event)
{
    auto parentWidgetWithClickableCardArray = dynamic_cast<QDialogWithClickableCardArray *>(parent());

    parentWidgetWithClickableCardArray->onCardClicked(this);
}

void ClickableCard::enterEvent(QEvent *event)
{
    auto parentWidgetWithClickableCardArray = dynamic_cast<QDialogWithClickableCardArray *>(parent());

    parentWidgetWithClickableCardArray->onCardHoverEnter(this);
}

void ClickableCard::leaveEvent(QEvent *event)
{
    auto parentWidgetWithClickableCardArray = dynamic_cast<QDialogWithClickableCardArray *>(parent());

    parentWidgetWithClickableCardArray->onCardHoverLeave(this);
}

int ClickableCard::getRotation(int drawPosition) const
{
    switch (drawPosition)
    {
    case DRAW_POSITION_MAIN_WIDGET_CENTER_LEFT:
        return 90;
    case DRAW_POSITION_MAIN_WIDGET_CENTER_TOP:
        return 180;
    case DRAW_POSITION_MAIN_WIDGET_CENTER_RIGHT:
        return 270;
    #ifdef CPU_DEBUG
    case DRAW_POSITION_MAIN_WIDGET_LEFT:
        return 90;
    case DRAW_POSITION_MAIN_WIDGET_TOP:
        return 180;
    case DRAW_POSITION_MAIN_WIDGET_RIGHT:
        return 270;
    #endif
    default:
        return 0;
    }
}

CardKey::CardKey(Card pData) : data(pData)
{
}

bool CompareCardKey::operator()(const CardKey &a, const CardKey &b) const
{
    if (a.data.suit < b.data.suit)
    {
        return true;
    }
    else if (a.data.suit == b.data.suit)
    {
        if (a.data.value < b.data.value)
        {
            return true;
        }
    }

    return false;
}

CardPixmapKey::CardPixmapKey(Card pData, int pWidth, int pHeight, int pRotation) : data(pData),
                                                                                   width(pWidth),
                                                                                   height(pHeight),
                                                                                   rotation(pRotation)
{
}

bool CompareCardPixmapKey::operator()(const CardPixmapKey &a, const CardPixmapKey &b) const
{
    if (a.data.suit < b.data.suit)
    {
        return true;
    }
    else if (a.data.suit == b.data.suit)
    {
        if (a.data.value < b.data.value)
        {
            return true;
        }
        else if (a.data.value == b.data.value)
        {
            if (a.width < b.width)
            {
                return true;
            }
            else if (a.width == b.width)
            {
                if (a.height < b.height)
                {
                    return true;
                }
                else if (a.height == b.height)
                {
                    if (a.rotation < b.rotation)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

ClickableCardArray::ClickableCardArray(int pDrawPosition, QSize pSize, QDialogWithClickableCardArray *parent) : QWidget(parent)
{
    drawPosition = pDrawPosition;
    size = pSize;
}

void ClickableCardArray::rescale()
{
    CardVector cardArr;

    for (auto &clickableCard : clickableCards)
    {
        cardArr.push_back(clickableCard.data);
    }

    showCards(cardArr);
}

void ClickableCardArray::changeDrawPosition(int newDrawPosition)
{
    drawPosition = newDrawPosition;
}

void ClickableCardArray::showCards(const CardVector &cardArr, CardStyleMap *cardStyles)
{
    int n = (int)cardArr.size();

    clickableCards.clear();
    clickableCards.resize(n);

    for (auto i = 0; i < n; i++)
    {
        clickableCards[i].setParent(parentWidget());

        Card card = cardArr[i];
        string style = "";

        if (cardStyles)
        {
            style = (*cardStyles)[card];
        }

        if (style.empty())
        {
            style = "background-color: white; border: 2px solid"; // default style
        }

        clickableCards[i].setData(card, drawPosition, size, style);
        clickableCards[i].move(getCardPosition(i, n));
        clickableCards[i].showNormal();
    }
}

void ClickableCardArray::hideCards()
{
    clickableCards.clear();
}

QPoint ClickableCardArray::getCardPosition(int i, int n) const
{
    if (useDynamicPositioning())
    {
        auto WIN_DIMENSIONS = getWindowDimensions();
        auto VERTICAL_SHIFT = getVerticalShift();
        auto HORIZONTAL_SHIFT = getHorizontalShift();
        auto CARDGAP = getCardGap();

        auto WIN_WIDTH = WIN_DIMENSIONS.width();
        auto WIN_HEIGHT = WIN_DIMENSIONS.height();
        auto TOTAL_WIDTH = size.width() /*card width*/ + (n - 1) * CARDGAP;

        #ifdef CPU_DEBUG
        if (drawPosition == DRAW_POSITION_MAIN_WIDGET_LEFT)
            return {(WIN_WIDTH) / 2 + HORIZONTAL_SHIFT, (WIN_HEIGHT) / 2 + VERTICAL_SHIFT + i * CARDGAP};
        if (drawPosition == DRAW_POSITION_MAIN_WIDGET_TOP)
            return {(WIN_WIDTH - TOTAL_WIDTH) / 2 + i * CARDGAP + HORIZONTAL_SHIFT, (WIN_HEIGHT) / 2 + VERTICAL_SHIFT};
        if (drawPosition == DRAW_POSITION_MAIN_WIDGET_RIGHT)
            return {(WIN_WIDTH) / 2 + HORIZONTAL_SHIFT, (WIN_HEIGHT) / 2 + VERTICAL_SHIFT + i * CARDGAP};
        #endif

        return {(WIN_WIDTH - TOTAL_WIDTH) / 2 + i * CARDGAP + HORIZONTAL_SHIFT, (WIN_HEIGHT) / 2 + VERTICAL_SHIFT};
    }
    else // static positioning
    {
        switch (drawPosition)
        {
        case DRAW_POSITION_MAIN_WIDGET_CENTER_BOTTOM:
            return {525, 375};
        case DRAW_POSITION_MAIN_WIDGET_CENTER_LEFT:
            return {325, 300};
        case DRAW_POSITION_MAIN_WIDGET_CENTER_TOP:
            return {525, 180};
        case DRAW_POSITION_MAIN_WIDGET_CENTER_RIGHT:
            return {655, 300};
        case DRAW_POSITION_MIDDLE_DLG_PARTNER:
            return {510, 260};
        case DRAW_POSITION_GAME_INFO_WIDGET:
            return {145, 35};
        case DRAW_POSITION_MESSAGE_BOX:
            return {75, 50};
        default: // not implemented
            return {0, 0};
        }
    }
}

bool ClickableCardArray::useDynamicPositioning() const
{
    switch (drawPosition)
    {
    case DRAW_POSITION_MAIN_WIDGET_BOTTOM:
    case DRAW_POSITION_MAIN_WIDGET_CENTER:
    case DRAW_POSITION_MIDDLE_DLG_NEST:
    case DRAW_POSITION_NEST_DLG_TOP:
    case DRAW_POSITION_NEST_DLG_BOTTOM:
    case DRAW_POSITION_PARTNER_DLG:
    case DRAW_POSITION_MESSAGE_BOX_NEST:
        return true;
    #ifdef CPU_DEBUG
    case DRAW_POSITION_MAIN_WIDGET_LEFT:
    case DRAW_POSITION_MAIN_WIDGET_TOP:
    case DRAW_POSITION_MAIN_WIDGET_RIGHT:
        return true;
    #endif
    default:
        return false; // dynamic positioning not implemented
    }
}

QSize ClickableCardArray::getWindowDimensions() const
{
    switch (drawPosition)
    {
    case DRAW_POSITION_MAIN_WIDGET_BOTTOM:
    case DRAW_POSITION_MAIN_WIDGET_CENTER:
        return MAIN_WINDOW_SIZE;
    case DRAW_POSITION_MIDDLE_DLG_NEST:
        return MIDDLE_DIALOG_SIZE;
    case DRAW_POSITION_NEST_DLG_TOP:
    case DRAW_POSITION_NEST_DLG_BOTTOM:
        return NEST_DIALOG_SIZE;
    case DRAW_POSITION_PARTNER_DLG:
        return PARTNER_DIALOG_SIZE;
    case DRAW_POSITION_MESSAGE_BOX_NEST:
        return MESSAGE_BOX_SIZE;
    #ifdef CPU_DEBUG
    case DRAW_POSITION_MAIN_WIDGET_LEFT:
        return MAIN_WINDOW_SIZE;
    case DRAW_POSITION_MAIN_WIDGET_TOP:
        return MAIN_WINDOW_SIZE;
    case DRAW_POSITION_MAIN_WIDGET_RIGHT:
        return MAIN_WINDOW_SIZE;
    #endif
    default:
        return QSize(0, 0); // dynamic positioning not implemented
    }
}

int ClickableCardArray::getVerticalShift() const
{
    switch (drawPosition)
    {
    case DRAW_POSITION_MAIN_WIDGET_BOTTOM:
        return 170;
    case DRAW_POSITION_MAIN_WIDGET_CENTER:
        return -100;
    case DRAW_POSITION_MIDDLE_DLG_NEST:
        return -200;
    case DRAW_POSITION_NEST_DLG_TOP:
        return -200;
    case DRAW_POSITION_NEST_DLG_BOTTOM:
        return 25;
    case DRAW_POSITION_PARTNER_DLG:
        return -40;
    case DRAW_POSITION_MESSAGE_BOX_NEST:
        return -70;
    #ifdef CPU_DEBUG
    case DRAW_POSITION_MAIN_WIDGET_LEFT:
        return -250;
    case DRAW_POSITION_MAIN_WIDGET_TOP:
        return -425;
    case DRAW_POSITION_MAIN_WIDGET_RIGHT:
        return -250;
    #endif
    default:
        return 0; // dynamic positioning not implemented
    }
}

int ClickableCardArray::getHorizontalShift() const
{
    switch (drawPosition)
    {
    case DRAW_POSITION_MAIN_WIDGET_BOTTOM:
    case DRAW_POSITION_MAIN_WIDGET_CENTER:
        return 45;
    case DRAW_POSITION_MIDDLE_DLG_NEST:
        return 230;
    case DRAW_POSITION_NEST_DLG_TOP:
    case DRAW_POSITION_NEST_DLG_BOTTOM:
        return 35;
    case DRAW_POSITION_PARTNER_DLG:
        return 0;
    case DRAW_POSITION_MESSAGE_BOX_NEST:
        return 15;
    #ifdef CPU_DEBUG
    case DRAW_POSITION_MAIN_WIDGET_LEFT:
        return -550;
    case DRAW_POSITION_MAIN_WIDGET_TOP:
        return 30;
    case DRAW_POSITION_MAIN_WIDGET_RIGHT:
        return 400;
    #endif
    default:
        return 0; // dynamic positioning not implemented
    }
}

int ClickableCardArray::getCardGap() const
{
    switch (drawPosition)
    {
    case DRAW_POSITION_MAIN_WIDGET_BOTTOM:
    case DRAW_POSITION_MAIN_WIDGET_CENTER:
    case DRAW_POSITION_MIDDLE_DLG_NEST:
    case DRAW_POSITION_NEST_DLG_TOP:
    case DRAW_POSITION_NEST_DLG_BOTTOM:
        return 40;
    case DRAW_POSITION_PARTNER_DLG:
        return 55;
    case DRAW_POSITION_MESSAGE_BOX_NEST:
        return 40;
    #ifdef CPU_DEBUG
    case DRAW_POSITION_MAIN_WIDGET_LEFT:
    case DRAW_POSITION_MAIN_WIDGET_TOP:
    case DRAW_POSITION_MAIN_WIDGET_RIGHT:
        return 40;
    #endif
    default:
        return 0; // only one card shown for this draw position
    }
}

QDialogWithClickableCardArray::QDialogWithClickableCardArray(bool pFixedSize, QWidget *parent) : ScaledQDialog(pFixedSize, parent)
{
}
