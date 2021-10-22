#ifndef GRAPHICSPAINTER_H
#define GRAPHICSPAINTER_H
#include <QWidget>

class GraphicsPainter : public QWidget
{
    Q_OBJECT
public:
    explicit GraphicsPainter(QWidget *parent = nullptr);

    void SetDraw(bool bDraw);

signals:
    void singalDrawOver();

public slots:

protected:
    void paintEvent(QPaintEvent *);     //绘制
    void mousePressEvent(QMouseEvent *e);       //按下
    void mouseMoveEvent(QMouseEvent *e);        //移动
    void mouseReleaseEvent(QMouseEvent *e);     //松开
    void mouseDoubleClickEvent(QMouseEvent *event);        //双击


    bool bDraw;             //是否处于绘制状态
    bool bLeftClick;            //是否已经开始左键点击，同时标识是否开始进行绘制
    bool bMove;             //是否处于绘制时的鼠标移动状态

    QVector<QPointF> pointList;
    QPointF movePoint;
};

#endif // GRAPHICSPAINTER_H
