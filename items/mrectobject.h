#ifndef MRECTOBJECT_H
#define MRECTOBJECT_H

#include "mbaseobject.h"

#include <QPainter>

class MRectObject Q_DECL_FINAL: public MBaseObject
{
public:
    explicit MRectObject(MRectObject *parent = nullptr);
    ~MRectObject() Q_DECL_OVERRIDE;

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    MBaseObject::OBJTYPE objType() const  Q_DECL_OVERRIDE { return MBaseObject::Rect; }

};

#endif // MRECTOBJECT_H
