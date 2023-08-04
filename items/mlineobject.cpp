#include "mlineobject.h"

#include <QGraphicsScene>

MLineObject::MLineObject(MBaseObject *parent)
    :MBaseObject (parent)
    , mStartPos(0,0)
    , mEndPos(0,0)
//    , Node_A(nullptr)
//    , Node_B(nullptr)

{
    setZValue(-2);
}

MLineObject::~MLineObject()
{

}

void MLineObject::operator=(const MLineObject &obj)
{
    mStartPos = obj.mStartPos;
    mEndPos = obj.mEndPos;
}

void MLineObject::setStartPoint(const QPointF &sp)
{
    mStartPos= sp;
}

void MLineObject::setEndPoint(const QPointF &ep)
{
    mEndPos = ep;
}

const QPointF &MLineObject::startPoint() const
{
    return mStartPos;
}

const QPointF &MLineObject::endPoint() const
{
    return mEndPos;
}

QRectF MLineObject::boundingRect() const
{
    qreal w = qAbs(mEndPos.x() - mStartPos.x());
    qreal h = qAbs(mEndPos.y() - mStartPos.y());

    qreal xDiff = mStartPos.x() - mEndPos.x();
    qreal yDiff = mStartPos.y() - mEndPos.y();

    (w-h < 0) ? bFlag = false : bFlag = true; /* 判断绘制轨迹 */

    unsigned int x = 1, y = 1;

    if(xDiff > 0) x = x << 0;
    else if(xDiff == 0.) x = x << 1;
    else x = x << 2;

    if(yDiff > 0) y = y << 4;
    else if(yDiff == 0.) y = y << 5;
    else y = y << 6;

    mState = State(x|y);
    QRectF r;
    switch (mState) {
    case BT_RL:
        r = QRectF(-w,-h, w, h);
        break;
    case BT_EE:
        r = QRectF(-1,-h,2,h);
        break;
    case BT_LR:
        r = QRectF(0,-h, w, h);
        break;
    case EE_RL:
        r = QRectF(-w, -1, w, 2);
        break;
    case EE_EE:
        r = QRectF(-1,-1,2,2);
        break;
    case EE_LR:
        r = QRectF(0,-1,w,2);
        break;
    case TB_RL:
        r = QRectF(-w, 0, w, h);
        break;
    case TB_EE:
        r = QRectF(-1, 0, 2, h);
        break;
    case TB_LR:
        r = QRectF(0,0,w,h);
        break;
    default: r = QRectF(-1,-1,2,2);break;
    }

    return r;
}

QPainterPath MLineObject::shape() const
{
    QPainterPath path;

    QRectF r = boundingRect();
    const QPointF points[4] = {
        r.topLeft(),
        r.topRight(),
        r.bottomRight(),
        r.bottomLeft()
    };
    switch (mState) {
    case BT_RL:
        if(bFlag){
            path.moveTo(points[2]);
            path.lineTo(points[3]);
            path.lineTo(points[0]);
        }
        else{
            path.moveTo(points[2]);
            path.lineTo(points[1]);
            path.lineTo(points[0]);
        }
        break;
    case BT_EE:
        path.moveTo(0,0);
        path.lineTo(0,r.top());
        break;
    case BT_LR:
        if(bFlag){
            path.moveTo(points[3]);
            path.lineTo(points[2]);
            path.lineTo(points[1]);
        }
        else{
            path.moveTo(points[3]);
            path.lineTo(points[0]);
            path.lineTo(points[1]);
        }
        break;
    case EE_RL:
        path.moveTo(0,0);
        path.lineTo(r.left(),0);
        break;
    case EE_EE:
        path.moveTo(0,0);
        break;
    case EE_LR:
        path.moveTo(0,0);
        path.lineTo(r.right(),0);
        break;
    case TB_RL:
        if(bFlag){
            path.moveTo(points[1]);
            path.lineTo(points[0]);
            path.lineTo(points[3]);
        }
        else{
            path.moveTo(points[1]);
            path.lineTo(points[2]);
            path.lineTo(points[3]);
        }
        break;
    case TB_EE:
        path.moveTo(0,0);
        path.lineTo(0,r.bottom());
        break;
    case TB_LR:
        if(bFlag){
            path.moveTo(points[0]);
            path.lineTo(points[1]);
            path.lineTo(points[2]);
        }
        else{
            path.moveTo(points[0]);
            path.lineTo(points[3]);
            path.lineTo(points[2]);
        }
        break;
    }
    return path;
}

void MLineObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(gPen); /* 设置画笔颜色 */

    QRectF r = boundingRect();
    const QPointF points[4] = {
        r.topLeft(),
        r.topRight(),
        r.bottomRight(),
        r.bottomLeft()
    };
    switch (mState) {
    case BT_RL:
        if(bFlag){
            painter->drawLine(points[2], points[3]);
            painter->drawLine(points[3], points[0]);
        }
        else{
            painter->drawLine(points[2], points[1]);
            painter->drawLine(points[1], points[0]);
        }
        break;
    case BT_EE:
        painter->drawLine(0,0,0,r.top());
        break;
    case BT_LR:
        if(bFlag){
            painter->drawLine(points[3], points[2]);
            painter->drawLine(points[2], points[1]);
        }
        else{
            painter->drawLine(points[3], points[0]);
            painter->drawLine(points[0], points[1]);
        }
        break;
    case EE_RL:
        painter->drawLine(0,0,r.left(),0);
        break;
    case EE_EE:
        painter->drawLine(0,0,0,0);
        break;
    case EE_LR:
        painter->drawLine(0,0,r.right(),0);
        break;
    case TB_RL:
        if(bFlag){
            painter->drawLine(points[1], points[0]);
            painter->drawLine(points[0], points[3]);
        }
        else{
            painter->drawLine(points[1], points[2]);
            painter->drawLine(points[2], points[3]);
        }
        break;
    case TB_EE:
        painter->drawLine(0,0,0,r.bottom());
        break;
    case TB_LR:
        if(bFlag){
            painter->drawLine(points[0], points[1]);
            painter->drawLine(points[1], points[2]);
        }
        else{
            painter->drawLine(points[0], points[3]);
            painter->drawLine(points[3], points[2]);
        }
        break;
    default: qWarning("no this state");break;
    }





    if(this->isSelected()) /* 选中时绘制外接虚线框 */
        drawSelectedRect(painter);

}

