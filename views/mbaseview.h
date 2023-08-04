#ifndef QGRAPHICSBASEVIEW_H
#define QGRAPHICSBASEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QMenu>
#include <QColorDialog>

#include "mlineobject.h"

class MBaseView : public QGraphicsView
{
public:

    QPointF gEndPoint;
    static QColor gXYLineColor;

    explicit MBaseView(QWidget *parent = nullptr);
    explicit MBaseView(QGraphicsScene *scene, QWidget *parent = nullptr);

    void init(void);

    void enbaleDrawLine(bool flag);
protected:

    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);

private slots:
    void slotChangeBkColor(void);

private:
    QTimer *pTimer;
    bool IS_MOVE;
    bool IS_MOVED;

    QPointF mLastPoint;

    QMenu *pMenu;
    QAction *pChangeBkColor;

    QColor mDefaultBkColor;

    QGraphicsLineItem *pXLine, *pYLine;

    bool bDrawLine;
    bool bIsDrawingLine = false;/* 画线状态，true正在画线，false画线完成或未画线 */
    MLineObject *mLineObject;



};

#endif // QGRAPHICSBASEVIEW_H
