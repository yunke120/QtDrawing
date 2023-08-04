#include "mBaseScene.h"
#include <QtMath>
#include <QPainter>
#include <QDebug>
#include <QGraphicsView>

int MBaseScene::gGridSize = 10;
int MBaseScene::gGridSquares = 5;
QColor MBaseScene::gBackgroundColor = QColor("#393939");
QColor MBaseScene::gForegroundColorDark = QColor("#292929");
QColor MBaseScene::gForegroundColorLight = QColor("#2f2f2f");
MBaseScene::MBaseScene(QObject *parent)
    : QGraphicsScene(parent)
{

}

MBaseScene::~MBaseScene()
{

}

void MBaseScene::keyPressEvent(QKeyEvent *event)
{
    QGraphicsScene::keyPressEvent(event);
}

void MBaseScene::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        QList<QGraphicsItem *> list = this->selectedItems();
        foreach(QGraphicsItem *item, list)
        {
            this->removeItem(item);
        }
    }
    else {
        QGraphicsScene::keyReleaseEvent(event);
    }
}

void MBaseScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setBrush(MBaseScene::gBackgroundColor);
    painter->setPen(MBaseScene::gBackgroundColor);
    painter->drawRect(rect);

    int left = qFloor(rect.left()); /* 向下取整 */
    int right = qCeil(rect.right()); /* 向上取整 */
    int top = qFloor(rect.top());
    int bottom = qCeil(rect.bottom());

    int _left = left - left % MBaseScene::gGridSize;
    int _top  = top  - top  % MBaseScene::gGridSize; /* 左上角第一个交叉点 */

    QVector<QLine> light_lines, dark_lines;
    int wh = MBaseScene::gGridSize * MBaseScene::gGridSquares;

    for (int i = _left; i <= right; i += MBaseScene::gGridSize)
    {
        if(i % wh == 0)
            dark_lines.append(QLine(i, top, i, bottom));
        else
            light_lines.append(QLine(i, top, i, bottom));
    }

    for (int j = _top; j <= bottom; j += MBaseScene::gGridSize) {
        if(j % wh == 0)
            dark_lines.append(QLine(left, j, right, j));
        else
            light_lines.append(QLine(left, j, right, j));
    }

    painter->setBrush(Qt::NoBrush);
    painter->setPen(MBaseScene::gForegroundColorLight);
    painter->drawLines(light_lines);

    painter->setPen(MBaseScene::gForegroundColorDark);
    painter->drawLines(dark_lines);
}

void MBaseScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MBaseScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MBaseScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
