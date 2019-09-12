#include "commonWidgets.h"

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    emit clicked(this);
}