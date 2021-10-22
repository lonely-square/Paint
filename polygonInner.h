#ifndef POLYGONINNER_H
#define POLYGONINNER_H
#include <QPoint>
#include <QVector>
#include <QList>
#include <QColor>
#include <QPainter>
#include <algorithm>
#include <QStack>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QTime>
#include <QCoreApplication>
using namespace std;
#include "edge.h"

class polygonInner
{
private:

    QColor color = Qt::red;             //默认红色
    QVector<QList<edge>> ET; //边表
    QPoint seed;
    int getMaxX();
    int getMinX();
    int getMaxY();
    int getMinY();
    void Sleep(int msec);

public:
    QVector<QPoint> points;

    void setPoint(QVector<QPoint> points);
    void showPolygonInEdgeTable();//建立有序边表算法
    void drawPolyScans(QPainter* ptr,QColor color,qreal scale); //扫描线算法
    void showPolygonInSeed(QPainter* ptr);//边标志算法
    bool isInPolygon(QPoint p);
};

#endif // POLYGONINNER_H
