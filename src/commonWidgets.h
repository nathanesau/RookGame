#ifndef COMMONWIDGETS_H
#define COMMONHWIDGETS_H

#include "common.h"

// custom widgets
class ClickableLabel : public QLabel
{
    Q_OBJECT

signals:
    void clicked(ClickableLabel *clickableLabel);

public:
    using QLabel::QLabel;

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif