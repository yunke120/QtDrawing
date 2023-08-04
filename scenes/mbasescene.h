#ifndef QGRAPHICSBASESCENE_H
#define QGRAPHICSBASESCENE_H

#include <QGraphicsScene>
#include <QKeyEvent>
class MBaseScene : public QGraphicsScene
{
public:
    explicit MBaseScene(QObject *parent = nullptr);
    ~MBaseScene();

    static int gGridSize; /* 单个小矩形的边长 */
    static int gGridSquares; /* 由深色线构成的矩形的边长由几个小矩形构成 */
    static QColor gBackgroundColor;
    static QColor gForegroundColorDark;
    static QColor gForegroundColorLight;
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);


};

#endif // QGRAPHICSBASESCENE_H
