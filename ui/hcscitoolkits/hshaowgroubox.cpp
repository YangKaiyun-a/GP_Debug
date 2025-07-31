#include "hshaowgroubox.h"

#include <QGraphicsOpacityEffect>

const QColor SHADOW_COLOR = QColor("#3c3c3c");

HShaowGrouBox::HShaowGrouBox(QWidget* parent)
    :QGroupBox(parent)
{
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(6, 8);//阴影偏移位置
    shadow_effect->setColor(SHADOW_COLOR);
    shadow_effect->setBlurRadius(8);//阴影圆角
    setGraphicsEffect(shadow_effect);
}
