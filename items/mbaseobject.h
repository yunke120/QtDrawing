#ifndef CIRCULTBASEITEM_H
#define CIRCULTBASEITEM_H

#include <QGraphicsObject>
#include <QKeyEvent>
#include <QPen>

class MBaseObject: public QGraphicsObject
{
public:

    enum OBJTYPE{
        Base,
        Line,
        Rect,
        Text,
        Ellipse,
    };
    Q_ENUM(OBJTYPE)
    explicit MBaseObject(QGraphicsObject *parent = nullptr);
    ~MBaseObject() override;

    // Dynamic cast
    bool isLine() const                     { return objType() == MBaseObject::Line; }
    bool isRect() const                     { return objType() == MBaseObject::Rect; }
    bool isText() const                     { return objType() == MBaseObject::Text; }
    bool isEllipse() const                  { return objType() == MBaseObject::Ellipse; }

    virtual void drawSelectedRect(QPainter *painter); /* 画虚线框 */
    virtual MBaseObject::OBJTYPE objType() const { return MBaseObject::Base; }
    QString nameString() const;

    static QColor gBorderColor; /* 虚线框颜色 */
    static QColor gObjectColor; /* 图元颜色 */
    static qreal gMoveOffset; /* 每次移动的偏移量 */
    QPen gPen;


protected: 
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;



private:
    bool bSelected; /* 图元是否被选中 */
    QColor mObjectColor;
    qreal mPenWidthF;

};

#endif // CIRCULTBASEITEM_H
