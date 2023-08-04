#include "mrectobject.h"


MRectObject::MRectObject(MRectObject *parent)
    : MBaseObject (parent)
{
}

MRectObject::~MRectObject()
{

}

QRectF MRectObject::boundingRect() const
{
    return QRectF(-10,-10,20,20);
}

void MRectObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(gPen); /* 设置画笔颜色 */
    painter->drawRect(this->boundingRect());

    if(this->isSelected())
        drawSelectedRect(painter);
}
