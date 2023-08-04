#ifndef MELLIPSEOBJECT_H
#define MELLIPSEOBJECT_H


#include "mbaseobject.h"

#include <QPainter>

class MEllipseObject Q_DECL_FINAL: public MBaseObject
{
public:
    explicit MEllipseObject(MEllipseObject *parent = nullptr);
    ~MEllipseObject() Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

    MBaseObject::OBJTYPE objType() const Q_DECL_OVERRIDE { return MBaseObject::Ellipse; }
};

#endif // MELLIPSEOBJECT_H
