#include "mbaseview.h"
#include <QDebug>
#include "mbaseobject.h"

QColor MBaseView::gXYLineColor = QColor(0,255,255);

MBaseView::MBaseView(QWidget *parent)
    : QGraphicsView(parent)
    , IS_MOVE(false)
    , IS_MOVED(false)
    , mDefaultBkColor(QColor(218,218,218))
    , bDrawLine(false)

{
    init();
}

MBaseView::MBaseView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(parent)
    , IS_MOVE(false)
    , IS_MOVED(false)
    , mDefaultBkColor(QColor(218,218,218))
    , bDrawLine(false)
{
    this->setScene(scene);
    init();
}

void MBaseView::init()
{
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing
                         | QPainter::HighQualityAntialiasing | QPainter::LosslessImageRendering);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    pTimer = new QTimer(this);
    pTimer->setInterval(150);
    connect(pTimer, &QTimer::timeout, this, [=](){
        IS_MOVE = true;
        this->setCursor(Qt::OpenHandCursor);
        pTimer->stop();
    });

    connect(this,&QGraphicsView::customContextMenuRequested,[=](const QPoint &)
    {
        if(!IS_MOVED) /* 在150ms内（未进入拖动状态）松开右键 */
        {
            pMenu->exec(QCursor::pos());
        }
        else IS_MOVED = false;
    });
    pMenu = new QMenu(this);
    pChangeBkColor = new QAction("Change Background Color", this);
    pMenu->addAction(pChangeBkColor);

    connect(pChangeBkColor, &QAction::triggered, this, &MBaseView::slotChangeBkColor);


    pXLine = new QGraphicsLineItem();
    pYLine = new QGraphicsLineItem();
    pXLine->setLine(-50,0,50,0);
    pYLine->setLine(0,-50,0,50);
    pYLine->setVisible(false);
    pXLine->setVisible(false);
    pXLine->setZValue(-10);
    pYLine->setZValue(-10);
}

void MBaseView::enbaleDrawLine(bool flag)
{
    bDrawLine = flag;
    IS_MOVED  = true;
//    this->setCursor(Qt::CrossCursor);

    /* ！！！ 此处应该用浅拷贝 */
//    void *ptr = item;
//    qDebug() << (int *)&ptr;
//    mLineObject = (MLineObject *)ptr;
//    mLineObject->setPos(gEndPoint);
    QPen pen(Qt::CustomDashLine);
    QVector<qreal> dashes;
    dashes << 2 << 4;
    pen.setDashPattern(dashes);
    pen.setColor(MBaseView::gXYLineColor);
    pen.setWidth(0);
    pXLine->setPen(pen);
    pYLine->setPen(pen);

    pXLine->setLine(this->sceneRect().left(),0,this->sceneRect().right(),0);
    pYLine->setLine(0,this->sceneRect().top(),0,this->sceneRect().bottom());

    this->scene()->addItem(pXLine);
    this->scene()->addItem(pYLine);
}



void MBaseView::mouseMoveEvent(QMouseEvent *event)
{
    if(IS_MOVE)
    {
        IS_MOVED = true;
        this->setCursor(Qt::ClosedHandCursor);
        QPointF posOffset = event->pos() - mLastPoint;
        mLastPoint = event->pos();

        int w = viewport()->rect().width() / 2;
        int h = viewport()->rect().height() / 2; // 获取View的中心

        QPoint newCenter(w - posOffset.x()+0.5,  h - posOffset.y()+0.5);
        centerOn(mapToScene(newCenter));
        this->scene()->update(); // 移动时刷新scene，不然背景会花
    }
    else if(bDrawLine) // 显示十字线
    {
        if(bIsDrawingLine)
            mLineObject->setEndPoint(mapToScene(event->pos()));
        pXLine->setPos(mapToScene(event->pos()));
        pYLine->setPos(mapToScene(event->pos()));
    }

    QGraphicsView::mouseMoveEvent(event);
}

void MBaseView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        pTimer->start();
        mLastPoint = event->pos();
    }
    else if (event->button() == Qt::LeftButton && !bDrawLine) {
        QGraphicsItem *item = itemAt(event->pos()); // 设置点击的图元被选中
        if(item != nullptr)
            item->setSelected(true);
        gEndPoint = mapToScene(event->pos()); /* 左键点击后，添加的图元会显示在此位置 */
        QGraphicsView::mousePressEvent(event);
    }
}

void MBaseView::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    if(event->button() == Qt::RightButton)
    {
        if(bDrawLine && !IS_MOVE) /* 右键取消十字线 */
        {
            bDrawLine = false;
            pXLine->setVisible(false);
            pYLine->setVisible(false);
            this->scene()->removeItem(pXLine);
            this->scene()->removeItem(pYLine);

            if(bIsDrawingLine) /* 右键按下时如果正在画线，则取消画线 */
            {
                bIsDrawingLine = false;
                this->scene()->removeItem(mLineObject);
            }
        }

        if(pTimer->isActive()) pTimer->stop();
        IS_MOVE = false;

    }
    else if (event->button() == Qt::LeftButton) {
        if(bDrawLine && (!bIsDrawingLine))
        {
            bIsDrawingLine = true; /* 左键点击开始画线 */
            MLineObject *item = new MLineObject();

            QPointF p = mapToScene(event->pos());
            item->setStartPoint(p);
            item->setEndPoint(p);
            item->setPos(p.x(), p.y());
            this->scene()->addItem(item);

            mLineObject = item; /* 浅拷贝 */
        }
        else if (bDrawLine && bIsDrawingLine) { /* 正在画线中点击左键会接着画 */

            MLineObject *item = new MLineObject();

            QPointF p = mapToScene(event->pos());
            item->setStartPoint(p);
            item->setEndPoint(p);
            item->setPos(p.x(), p.y());
            this->scene()->addItem(item);

            mLineObject = item; /* 浅拷贝 */
        }
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void MBaseView::wheelEvent(QWheelEvent *event)
{
    event->delta() > 0 ? scale(1.05,1.05) : scale(0.95,0.95);
}

void MBaseView::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    if(!bDrawLine) return;
    pXLine->setVisible(true);
    pYLine->setVisible(true);
}

void MBaseView::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    if(!bDrawLine) return;
    pXLine->setVisible(false);
    pYLine->setVisible(false);
}

void MBaseView::slotChangeBkColor()
{
    mDefaultBkColor = QColorDialog::getColor(Qt::white, this);
    this->scene()->setBackgroundBrush(mDefaultBkColor);
}


