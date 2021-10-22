#include "graphicspainter.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

GraphicsPainter::GraphicsPainter(QWidget *parent) : QWidget(parent)
{
    //填充背景色
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

    bDraw = false;
    bLeftClick = false;
    bMove = false;
    setMouseTracking(true);
}

void GraphicsPainter::SetDraw(bool bDraw)
{
    this->bDraw = bDraw;
    pointList.clear();
}

//重新实现paintEvent
void GraphicsPainter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(bDraw)
    {
       painter.setPen(QColor(255,0,0));
       QVector<QLineF> lines;
       for(int i = 0; i<pointList.size()-1; i++)
       {
           QLineF line(QPointF(pointList[i].x(), pointList[i].y()), QPointF(pointList[i+1].x(), pointList[i+1].y()));
           lines.push_back(line);
       }
       if(bMove&&pointList.size()>0)
       {
           QLineF line(QPointF(pointList[pointList.size()-1].x(), pointList[pointList.size()-1].y()), movePoint);
           lines.push_back(line);
       }
       painter.drawLines(lines);
    }
}

//按下
void GraphicsPainter::mousePressEvent(QMouseEvent *e)
{
    if(bDraw)
    {
        if(!bLeftClick)
        {
            pointList.clear();
            bLeftClick = true;
        }
    }
    //qDebug()<<"Press";
}

//移动
void GraphicsPainter::mouseMoveEvent(QMouseEvent *e)
{
    if(bDraw&&bLeftClick)
    {
        movePoint = e->pos();
        bMove = true;
        this->update();
    }
    //qDebug()<<"Move";
}

//松开
void GraphicsPainter::mouseReleaseEvent(QMouseEvent *e)
{
    if(bDraw&&bLeftClick)
    {
        pointList.push_back(QPointF(e->x(), e->y()));
        bMove = false;
        this->update();
    }
    //qDebug()<<"Release";
}

//双击
void GraphicsPainter::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(bDraw)
    {
        bLeftClick = false;
        pointList.push_back(pointList[0]);
        this->update();
        singalDrawOver();
    }
    //qDebug()<<"DoubleClick";
}
