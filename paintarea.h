#ifndef PAINTAREA_H
#define PAINTAREA_H
#include <iostream>
#include <QWidget>
#include <QStack>
#include <QBrush>
#include <QColor>
#include<vector>
#include "shape.h"


struct Line{
    QPoint beginPoint;
    QPoint endPoint;
};





class PaintArea : public QWidget
{
    Q_OBJECT

public:
    enum DrawShapeType{ //形状种类
        EnumDrawNone,
        EnumDrawLinecenter,
        EnumDrawLineBresenham,
        EnumDrawRect,
        EnumDrawCircle,
        EnumDrawEllipse,
        EnumDrawPolygon,
        EnumScanFill,
        EnumSeedFill,
        EnumFillRectangle,
        EnumPologonScan,
        EnumCohenSutherland,
        EnumCutLineCenter,
        EnumTransShape,  //平移模式
        EnumRotateShape,
        EnumZoomShape,
    };


    Shape::DrawLineType m_drawLineType= Shape::defaultLine;
    void setLineShape(Shape::DrawLineType drawLinrType);



    explicit PaintArea(QWidget *parent = 0,Pencil *pencil1 = nullptr);
    virtual ~PaintArea();

    void setImage(QString fileName);
    QImage getImage();

    void setImageSize(QSize size);
    QSize getImageSize();

    void setImageBgColor(QColor color);
    QColor getImageBgColor();

    void setDrawShape(DrawShapeType drawShapeType);
    void setDrawShapeColor(QColor color);

    bool getIsModify();

    void zoomIn();
    void zoomOut();
    void restore();
    void clear();
    void choose();
    void rotate();
    void zoom();

    void chooseshape(QPoint pos);


    void paintLineBresenham(QPoint beginPoint, QPoint endPoint,QPen p,int width, QImage &image);  //保留一个用于辅助作图

    void pologonScan(QPoint beginPoint, QPoint endPoint, QImage &image);

    void paintPolygon(QVector<QPoint> pointList,QPen p, QImage &image);

    void paintTrans(QImage &image);   //绘制平移图形
    void paintRotate(double thita,QImage &image);
    void paintZoom(double x,double y,QImage &image);

    void fillbyScan(QPoint beginPoint, QPoint endPoint, QImage &image);
    void fillbySeed(QPoint beginPoint, QColor newcolor,QColor oldColor,QImage &image);
    void FloodFill4(int x, int y, QColor oldColor, QColor newColor, QImage &image);
    void CohenSutherland(float x1,float y1,float x2,float y2,float xl,float yb,float xr,float yt, QImage &image);
    void Middleland(float x1,float y1,float x2,float y2,float xl,float yb,float xr,float yt, QImage &image);
    void paintImage(QImage &image);
    void setPen(QPen,int);//设置画笔
    void setBrush(QBrush, int);


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);//鼠标点击、移动、松开
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent * ev);

private:
    double thita;  //旋转用
    double sX;  //缩放用
    double sY;
    int width=1;  //笔宽

    QPoint   deltaPoint;
    QImage    m_image;
    QImage    m_tmpImage;
    QImage    m_tmpImageTrans;   //用于变换图元时，保存非变换图元的视图
    QColor    m_imageBgColor;

    QPen      pen;     //存储用户设置的画笔数据
    QBrush    brush;

    qreal     m_scale;

    bool      m_isChoose;//是否处于选择模式
    bool      m_isDraw;
    bool      m_isModify;
    bool      m_isMove;
    QVector<QPoint> pointList;//这两项是为了实现多边形
    QPoint    m_beginPoint;
    QPoint    m_movePoint;
    QPoint    m_endPoint;
    QPoint    m_choosePoint;

    DrawShapeType   m_drawShapeType;

    Shape* m_currShapes;  //当前绘画图元
    Shape* m_assistShapes;  //当前辅助图元

    QVector<Shape*>         m_shapes;
    QVector<Linecenter*>    m_linecenters;
    QVector<LineBresenham*> m_lineBresenhams;



    //存储

    Pencil* pencil;    //自定义的画笔

};

#endif // PAINTAREA_H
