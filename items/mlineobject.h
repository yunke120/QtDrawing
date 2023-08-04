#ifndef MLINEOBJECT_H
#define MLINEOBJECT_H

#include "mbaseobject.h"

#include <QPainter>
class MLineObject Q_DECL_FINAL: public MBaseObject
{
public:
    explicit MLineObject(MBaseObject *parent = nullptr);
    ~MLineObject() Q_DECL_OVERRIDE;
    void operator=(const MLineObject &obj);

    enum State{
        BT_RL = 0x11,
        BT_EE = 0x12,
        BT_LR = 0x14,

        EE_RL = 0x21,
        EE_EE = 0x22,
        EE_LR = 0x24,

        TB_RL = 0x41,
        TB_EE = 0x42,
        TB_LR = 0x44
    };

    void setStartPoint(const QPointF &sp);
    void setEndPoint(const QPointF &ep);
    const QPointF &startPoint() const;
    const QPointF &endPoint() const;

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

    MBaseObject::OBJTYPE objType() const Q_DECL_OVERRIDE { return MBaseObject::Line; }

private:
    QPointF mStartPos;
    QPointF mEndPos;

    mutable State mState;
    mutable bool bFlag;

//    QPointF *Node_A, *Node_B;
};

#endif // MLINEOBJECT_H
