#ifndef SHAPE_H
#define SHAPE_H

#include <QPoint>
#include <iostream>
#include <QVector>
#include "polygonInner.h"
#include "pencil.h"
#include <math.h>
#include <Eigen>
#include <pencil.h>


class Shape{
    //所有图元的基类
public:
    bool ischoose=false;
    QPoint center;
    QPen p;

    int width;

    void setcenter();

    void paintchoose(){

        width+=10;


    };

    void paintdischoose(){

        width-=10;
    };



    double dis(QPoint pos){
      int x=center.x()-pos.x();
      int y=center.y()-pos.y();

     return sqrt((x * x + y * y));

    }

    virtual bool IsIn(QPoint) =0;  //点是否在里面
    virtual void DrawIt(QPainter* ptr,QColor color,qreal scale)=0;     //内部填充颜色

    virtual void translation(int offsetX,int offsetY);//平移
    virtual void rotate(double thita);//旋转
    virtual void zoom(double x,double y);//缩小

};

class Linecenter:public Shape{
public:
    QPoint beginPoint;
    QPoint endPoint;
    void setcenter(){
        int x=0,y=0;
        x=(beginPoint.x()+endPoint.x())/2;
        y=(beginPoint.y()+endPoint.y())/2;
        center=QPoint(x,y);
    }

    bool IsIn(QPoint);
    void DrawIt(QPainter* ptr,QColor color,qreal scale);


};

class Bezier:public Shape{
public:
    QVector<QPoint> fixPoints;  //固定点
    QVector<QPoint> resPoints;  //生成点

    void create();
    void setcenter(){

    }

    bool IsIn(QPoint);
    void DrawIt(QPainter* ptr,QColor color,qreal scale);


};

class LineBresenham: public Shape{
public:
    QPoint beginPoint;
    QPoint endPoint;
    void setcenter(){
        int x=0,y=0;
        x=(beginPoint.x()+endPoint.x())/2;
        y=(beginPoint.y()+endPoint.y())/2;
        center=QPoint(x,y);

    }
    bool IsIn(QPoint);
    void DrawIt(QPainter* ptr,QColor color,qreal scale);
};


class Rect: public Shape{
public:
    QPoint beginPoint;
    QPoint endPoint;
    void setcenter(){
        int x=0,y=0;
        x=(beginPoint.x()+endPoint.x())/2;
        y=(beginPoint.y()+endPoint.y())/2;
        center=QPoint(x,y);

    }
    bool IsIn(QPoint);
    void DrawIt(QPainter* ptr,QColor color,qreal scale);
};

class Circle: public Shape{
public:
    QPoint beginPoint;
    QPoint endPoint;
    void setcenter(){
        int x=0,y=0;
        x=(beginPoint.x()+endPoint.x())/2;
        y=(beginPoint.y()+endPoint.y())/2;
        center=QPoint(x,y);

    }
    bool IsIn(QPoint);
    void DrawIt(QPainter* ptr,QColor color,qreal scale);

};

class ellipse: public Shape{
public:
    QPoint beginPoint;
    QPoint endPoint;

    bool IsIn(QPoint);
    void DrawIt(QPainter* ptr,QColor color,qreal scale);

    void setcenter(){
        int x=0,y=0;
        x=(beginPoint.x()+endPoint.x())/2;
        y=(beginPoint.y()+endPoint.y())/2;
        center=QPoint(x,y);

    }

};

class polygon: public Shape{
public:
    QVector<QPoint> pointList;
    polygonInner* polygonInner;

    bool IsIn(QPoint);
    void DrawIt(QPainter* ptr,QColor color,qreal scale);
    void setcenter(){
        int x=0,y=0;
        for(int i = 0; i<pointList.size()-1; i++)
        {
            x+=pointList[i].x();
            y+=pointList[i].y();
        }

        x/=(pointList.size()-1);
        y/=(pointList.size()-1);

        center=QPoint(x,y);

    }
    void translation(int offsetX,int offsetY);//平移
    void rotate(double thita);
    void zoom(double x,double y);
};


#endif // SHAPE_H
