#ifndef MTEXTOBJECT_H
#define MTEXTOBJECT_H

#include "mbaseobject.h"
#include <QGraphicsView>
#include <QKeyEvent>
#include <QFont>
#include <QPainter>

class MTextObject Q_DECL_FINAL: public MBaseObject
{
public:
    explicit MTextObject(MBaseObject *parent = nullptr);
    explicit MTextObject(const QString &text, MBaseObject *parent = nullptr);

    MTextObject& operator= (const MTextObject&);

    ~MTextObject() Q_DECL_OVERRIDE;

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

    MBaseObject::OBJTYPE objType() const Q_DECL_OVERRIDE { return MBaseObject::Text; }

    void setText(const QString &text) {mText = text;}
    const QString text() { return mText; }
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QFont mFont;
    QString mText;
    bool IS_Edit;
};

#endif // MTEXTOBJECT_H
