#include <QtWidgets>
#include <cstdio>
#include <paintarea.h>
#include <math.h>
#include<iostream>
#include <QDebug>

#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
vector<struct Line> ll1;
PaintArea::PaintArea(QWidget *parent,Pencil* pencil1)
    : QWidget(parent)
{
    m_image  = QImage(1500, 750, QImage::Format_RGB32);
    m_imageBgColor = QColor(255, 255, 255);
    m_image.fill(m_imageBgColor);

    m_scale = 1; //控制缩放


    m_isDraw = false;
    m_isModify=false;
    m_isChoose=false;
    m_beginPoint = QPoint(0, 0);
    m_endPoint = QPoint(0, 0);

    m_drawShapeType = EnumDrawLinecenter;

    pencil= pencil1;    //用户用于自定义的笔

}

PaintArea::~PaintArea()
{
    qDeleteAll(m_linecenters);
    qDeleteAll(m_lineBresenhams);
    qDeleteAll(m_shapes);

}

void PaintArea::setPen(QPen p,int value)
{
    width = value;
    pen = p;
    update();

}


void PaintArea::setImage(QString fileName)
{
    QImage image(fileName);
    QSize size = image.size();
    setImageSize(size);
    m_image = image;
    update();
}

QImage PaintArea::getImage()
{
    return m_image;
}

void PaintArea::setImageSize(QSize size)
{
    QImage image(size.width(), size.height(), QImage::Format_RGB32);
    m_image = image;
    update();
}

QSize PaintArea::getImageSize()
{
    return m_image.size();
}

void PaintArea::setImageBgColor(QColor color)
{
    m_imageBgColor = color;
    m_image.fill(m_imageBgColor);
    update();
}

QColor PaintArea::getImageBgColor()
{
    return m_imageBgColor;
}

//选择画画模式
void PaintArea::setDrawShape(DrawShapeType drawShapeType)
{
    m_drawShapeType = drawShapeType;

    if( drawShapeType == EnumZoomShape | drawShapeType == EnumRotateShape) //因为是键盘事件
    {

        m_tmpImageTrans = QImage(1500, 750, QImage::Format_RGB32);
        m_imageBgColor = QColor(255, 255, 255);
        m_tmpImageTrans.fill(m_imageBgColor);

        QPainter painter(&m_tmpImageTrans); //先将m_tmpImageTrans画上非选中图元以备用
        for(int i=0;i<m_shapes.size();i++)
        {
            if(!m_shapes[i]->ischoose)
            {
                m_shapes[i]->DrawIt(&painter,m_shapes[i]->p.color());
            }
        }
    }
    setFocus();
}

void PaintArea::setLineShape(Shape::DrawLineType drawLinrType)
{
    m_drawLineType=drawLinrType;
}



bool PaintArea::getIsModify()
{
    return m_isModify;
}


//缩放 放大
void PaintArea::zoomIn()
{
    m_scale *= 1.2;
    update();
}

//缩小
void PaintArea::zoomOut()
{
    m_scale /= 1.2;
    update();
}

void PaintArea::restore()
{
    m_scale = 1;
    update();
}
//


//清除
void PaintArea::clear()
{
    m_image.fill(m_imageBgColor);
    update();
}

void PaintArea::choose()
{
    if(m_isChoose==false)
    {m_isChoose = true;
  QMessageBox::information(NULL, "提示", "进入选择模式");

    }

  else{
      m_isChoose = false;
       QMessageBox::information(NULL, "提示", "退出选择模式");
    }
}


void PaintArea::rotate()
{
}

void PaintArea::zoom()
{
}

void PaintArea::chooseshape(QPoint pos){

    if(m_shapes.size()==0) {


        QMessageBox::information(NULL, "提示", "没有可以选中的图形");

        return;
    }


    for (int i = 0; i < m_shapes.size(); i++) {

        pos=pos/m_scale;
        if( m_shapes[i]->IsIn(pos) )  m_shapes[i]->ischoose=!m_shapes[i]->ischoose;
      }


}




void PaintArea::paintLineBresenham(QPoint beginPoint, QPoint endPoint,QPen p,int width, QImage &image)
{

          Pencil* pencil = new Pencil();
           pencil->width = width;
           QPainter paint(&image);
           paint.setPen(pen);


       if (m_drawLineType == Shape::dashLine)
           {

           int x0 = beginPoint.x()/ m_scale;
           int y0 = beginPoint.y()/ m_scale;
           int x1 = endPoint.x()/ m_scale;
           int y1 = endPoint.y()/ m_scale;

           int dx = x1 - x0;//x偏移量
           int dy = y1 - y0;//y偏移量
           int ux = dx >0 ? 1 : -1;//x伸展方向
           int uy = dy >0 ? 1 : -1;//y伸展方向
           int dx2 = abs(dx << 1);//x偏移量乘2
           int dy2 = abs(dy << 1);//y偏移量乘2
           int brokenFlag = 0;

           if (abs(dx)>abs(dy))//以x为增量方向计算
           {
               int e = -dx; //e = -0.5 * 2 * dx,把e 用2 * dx* e替换
               int x = x0;//起点x坐标
               int y = y0;//起点y坐标
               while (x!=x1+ux)
               {
                   brokenFlag++;
                   if ( brokenFlag%(width*6) >width*0 && brokenFlag%(width*6) < width*4 )

                   pencil->CirclePen(&paint,x,y);

                   e = e + dy2;//来自 2*e*dx= 2*e*dx + 2dy  （原来是 e = e + k）
                   if (e > 0)//e是整数且大于0时表示要取右上的点（否则是右下的点）
                   {
                       if (y!=y1)
                       {
                           y += uy;
                       }
                       e = e - dx2;//2*e*dx = 2*e*dx - 2*dx  (原来是 e = e -1)
                   }
                   x += ux;
               }
           }
           else
           {//以y为增量方向计算
               int e = -dy; //e = -0.5 * 2 * dy,把e 用2 * dy* e替换
               int x = x0;//起点x坐标
               int y = y0;//起点y坐标
               while (y!=y1+uy)
               {
                   brokenFlag++;
                   if ( brokenFlag%(width*6) >width*0 && brokenFlag%(width*6) < width*4 )
                   pencil->CirclePen(&paint,x,y);
                   e = e + dx2;//来自 2*e*dy= 2*e*dy + 2dy  （原来是 e = e + k）
                   if (e > 0)//e是整数且大于0时表示要取右上的点（否则是右下的点）
                   {
                       if (x!=x1)
                       {
                           x += ux;
                       }
                       e = e - dy2;//2*e*dy = 2*e*dy - 2*dy  (原来是 e = e -1)
                   }
                   y += uy;
               }
           }
       }
       else if  (m_drawLineType == Shape::defaultLine)
       {
       int x0 = beginPoint.x()/ m_scale;
       int y0 = beginPoint.y()/ m_scale;
       int x1 = endPoint.x()/ m_scale;
       int y1 = endPoint.y()/ m_scale;

       int dx = x1 - x0;//x偏移量
       int dy = y1 - y0;//y偏移量
       int ux = dx >0 ? 1 : -1;//x伸展方向
       int uy = dy >0 ? 1 : -1;//y伸展方向
       int dx2 = abs(dx << 1);//x偏移量乘2
       int dy2 = abs(dy << 1);//y偏移量乘2
       if (abs(dx)>abs(dy))//以x为增量方向计算
       {
           int e = -dx; //e = -0.5 * 2 * dx,把e 用2 * dx* e替换
           int x = x0;//起点x坐标
           int y = y0;//起点y坐标
           while (x!=x1+ux)
           {
               pencil->CirclePen(&paint,x,y);
               e = e + dy2;//来自 2*e*dx= 2*e*dx + 2dy  （原来是 e = e + k）
               if (e > 0)//e是整数且大于0时表示要取右上的点（否则是右下的点）
               {
                   if (y!=y1)
                   {
                       y += uy;
                   }
                   e = e - dx2;//2*e*dx = 2*e*dx - 2*dx  (原来是 e = e -1)
               }
               x += ux;
           }
       }
       else
       {//以y为增量方向计算
           int e = -dy; //e = -0.5 * 2 * dy,把e 用2 * dy* e替换
           int x = x0;//起点x坐标
           int y = y0;//起点y坐标
           while (y!=y1+uy)
           {

               pencil->CirclePen(&paint,x,y);
               e = e + dx2;//来自 2*e*dy= 2*e*dy + 2dy  （原来是 e = e + k）
               if (e > 0)//e是整数且大于0时表示要取右上的点（否则是右下的点）
               {
                   if (x!=x1)
                   {
                       x += ux;
                   }
                   e = e - dy2;//2*e*dy = 2*e*dy - 2*dy  (原来是 e = e -1)
               }
               y += uy;
           }
       }
   }

    m_isModify = true;
}



void PaintArea::paintPolygon(QVector<QPoint> pointList,QPen p, QImage &image)
{

           for(int i = 0; i<pointList.size()-1; i++)//画出前面的
           {

               PaintArea::paintLineBresenham(QPoint(pointList[i].x(), pointList[i].y()), QPoint(pointList[i+1].x(), pointList[i+1].y()),p,width,image);
           }

           if(m_isMove&&pointList.size()>0)
           {

               PaintArea::paintLineBresenham(QPoint(pointList[pointList.size()-1].x(), pointList[pointList.size()-1].y()), m_movePoint,p,width,image);
           }


             m_isModify = true;

 }

void PaintArea::fillbyScan(QPoint beginPoint, QPoint endPoint, QImage &image){

    QPainter paint(&image);

    paint.setPen(pen);


    for(int i=0;i<m_shapes.size();i++)
    {
        beginPoint=beginPoint/m_scale;

        if(m_shapes[i]->IsIn(beginPoint))
        {
            m_shapes[i]->DrawIt(&paint,pen.color());
        }
    }


}

void fillbySeedUpLeft(QPoint beginPoint, QColor newcolor, QColor oldColor, QImage &image)
{
    QPoint A(1,0);
    QPoint B(0,1);
    QColor color;
    color = image.pixel(beginPoint);
    if(beginPoint.x()<1500&&beginPoint.y()<750
            &&beginPoint.x()>0&&beginPoint.y()>0
            &&(color != newcolor )&&(color == oldColor)){

        image.setPixel(beginPoint, newcolor.rgb());
        fillbySeedUpLeft(beginPoint-B, newcolor,oldColor, image);
        fillbySeedUpLeft(beginPoint-A, newcolor,oldColor, image);
    }
}

//种子填充算法
void fillbySeedUpRight(QPoint beginPoint, QColor newcolor, QColor oldColor, QImage &image)
{
    QPoint A(1,0);
    QPoint B(0,1);
    QColor color;
    color = image.pixel(beginPoint);
    if(beginPoint.x()<1500&&beginPoint.y()<750
            &&beginPoint.x()>0&&beginPoint.y()>0
            &&(color != newcolor )&&(color == oldColor)){

        image.setPixel(beginPoint, newcolor.rgb());
        fillbySeedUpRight(beginPoint-B, newcolor,oldColor, image);
        fillbySeedUpRight(beginPoint+A, newcolor,oldColor, image);
    }
}

void fillbySeedDownLeft(QPoint beginPoint, QColor newcolor, QColor oldcolor, QImage &image)
{
    QPoint A(1,0);
    QPoint B(0,1);
    QColor color;
    color = image.pixel(beginPoint);
    if(beginPoint.x()<1500&&beginPoint.y()<750
            &&beginPoint.x()>0&&beginPoint.y()>0
            &&(color != newcolor )&&(color == oldcolor)){

        image.setPixel(beginPoint, newcolor.rgb());
        fillbySeedDownLeft(beginPoint+B, newcolor,oldcolor, image);
        fillbySeedDownLeft(beginPoint-A, newcolor,oldcolor, image);
    }
}

void fillbySeedDownRight(QPoint beginPoint, QColor newcolor, QColor oldColor , QImage &image)
{
    QPoint A(1,0);
    QPoint B(0,1);
    QColor color;
    color = image.pixel(beginPoint);
    if((color == oldColor)&&beginPoint.x()<1500&&beginPoint.y()<750
            &&beginPoint.x()>0&&beginPoint.y()>0
            &&(color != newcolor)){
        image.setPixel(beginPoint, newcolor.rgb());
        fillbySeedDownRight(beginPoint+B, newcolor,oldColor, image);
        fillbySeedDownRight(beginPoint+A, newcolor,oldColor, image);
    }
}

void PaintArea::fillbySeed(QPoint beginPoint, QColor newcolor, QColor oldColor,QImage &image){

    QPoint A(1,0);
    QPoint B(0,1);

    image.setPixel(beginPoint, newcolor.rgb());
    fillbySeedDownRight(beginPoint+B, newcolor,oldColor, image);
    fillbySeedUpLeft(beginPoint-B, newcolor,oldColor, image);
    fillbySeedDownLeft(beginPoint-A, newcolor,oldColor, image);
    fillbySeedUpRight(beginPoint+A, newcolor,oldColor, image);


}


//求交点
void crossX(int x1,int y1,int x2,int y2,int x,int &y){
    double a=x;
    y=qRound((y2-y1)*(a-x1)/(x2-x1)+y1);
}
void crossY(int x1,int y1,int x2,int y2,int &x,int y){
    double b=y;
    x=qRound((x2-x1)*(b-y1)/(y2-y1)+x1);
}
//CohenSuther裁剪
void PaintArea::CohenSutherland(float x1,float y1,float x2,float y2,float xmin,float ymin,float xmax,float ymax, QImage &image){

       int code1=0;
       int code2=0;
       //按区域编码（从编码位左到右依次是对应裁剪边界的上、下、右、左）
       if(x1<xmin) code1|=0x0001;
       if(x1>xmax) code1|=0x0010;
       if(y1<ymin) code1|=0x0100;
       if(y1>ymax) code1|=0x1000;
       if(x2<xmin) code2|=0x0001;
       if(x2>xmax) code2|=0x0010;
       if(y2<ymin) code2|=0x0100;
       if(y2>ymax) code2|=0x1000;
       //完全在窗口内的情况
       if((code1|code2)==0) {
           QPoint begin(x1,y1),end(x2,y2);
           paintLineBresenham(begin,end,pen,width,image);
           return ;
       }
       //完全在窗口外的情况
       if((code1&code2)!=0){
           x1=x2=y1=y2=-1;
           return;
       }
       //其余情况，按照上、下、右、左的顺序进行边界求交
       if(((code1|code2)&0x1000)!=0){
           int x3,y3=ymax;
           crossY(x1,y1,x2,y2,x3,y3);
           if(y1>y2){
               x1=x3;y1=y3;
           }
           else{
               x2=x3;y2=y3;
           }

       }
       else if(((code1|code2)&0x0100)!=0){
           int x3,y3=ymin;
           crossY(x1,y1,x2,y2,x3,y3);
           if(y1<y2){
               x1=x3;y1=y3;
           }
           else{
               x2=x3;y2=y3;
           }

       }
       else if(((code1|code2)&0x0010)!=0){
           int x3=xmax,y3;
           crossX(x1,y1,x2,y2,x3,y3);
           if(x1>x2){
               x1=x3;y1=y3;
           }
           else{
               x2=x3;y2=y3;
           }

       }
       else if(((code1|code2)&0x0001)!=0){
           int x3=xmin,y3;
           crossX(x1,y1,x2,y2,x3,y3);
           if(x1<x2){
               x1=x3;y1=y3;
           }
           else{
               x2=x3;y2=y3;
           }

       }
       return CohenSutherland(x1,y1,x2,y2,xmin,ymin,xmax,ymax,image);
}

int encode(int x,int y,int XL,int XR,int YB,int YT)//编码
{
    int c=0;
    if(x<XL) c|=LEFT;
    if(x>XR) c|=RIGHT;
    if(y<YB) c|=BOTTOM;
    if(y>YT) c|=TOP;
    return c;
}

void PaintArea::Middleland(float x1,float y1,float x2,float y2,float xmin,float ymin,float xmax,float ymax, QImage &image){

       int code1=0;
       int code2=0,code3;
       int x3,x4=x2,y3,y4=y2;
       float x,y,d;
       //按区域编码（从编码位左到右依次是对应裁剪边界的上、下、右、左）
       code1=encode(x1,y1,xmin,xmax,ymin,ymax);
       code2=encode(x2,y2,xmin,xmax,ymin,ymax);
       //完全在窗口内的情况
       if((code1|code2)==0) {
           QPoint begin(x1,y1),end(x2,y2);
           paintLineBresenham(begin,end,pen,width,image);
           return ;
       }
       //完全在窗口外的情况
       if((code1&code2)!=0){
           x1=x2=y1=y2=-1;
           return;
       }
       //其余情况，按照上、下、右、左的顺序进行边界求交
       while(1)//在线段与窗口有交的情况中，求出离P0最近的可见点
           {
               x=(x1+x2)/2.0;
               y=(y1+y2)/2.0;
               d=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
               if(d<=1)//精度为1个像素
               {
                   x1=x; y1=y;break;
               }
               code3=encode(x,y,xmin,xmax,ymin,ymax);
               if((code1 & code3) != 0)
               {
                   x1=x; y1=y; code1=encode(x1,y1,xmin,xmax,ymin,ymax);
               }
               else
               {
                   x2=x; y2=y;
               }
           }
           x3=x1;y3=y1;//保存求出的交点坐标
           x2=x4;y2=y4;//恢复P1的初值
           while(1) //在线段与窗口有交的情况中，求出离P1最近的可见点
           {
               x=(x1+x2)/2.0;
               y=(y1+y2)/2.0;
               d=sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
               if(d<=1)
               {
                   x2=x; y2=y;break;
               }
               code3=encode(x,y,xmin,xmax,ymin,ymax);
               if((code2 & code3) != 0)
               {
                   x2=x; y2=y; code2=encode(x2,y2,xmin,xmax,ymin,ymax);
               }
               else
               {
                   x1=x; y1=y;
               }
           }
           x1=x3;y1=y3;
           QPoint begin(x1,y1),end(x2,y2);
           paintLineBresenham(begin,end,pen,width,image);

           return;
}


void PaintArea::paintTrans(QPainter* paint,QVector<Shape*> m_shapes)  //画布和图元集
{
    for(int i=0;i<m_shapes.size();i++)
    {
        if (m_shapes[i]->ischoose)
        {
            paint->setPen(m_shapes[i]->p);
            m_shapes[i]->translation(deltaPoint.x(),deltaPoint.y());
            m_shapes[i]->DrawIt(paint,m_shapes[i]->p.color());
        }
    }
}

void PaintArea::paintRotate(double thita,QImage &image)
{
    qDebug() << "00019"<<endl;
    QPainter paint(&image);
    for(int i=0;i<m_shapes.size();i++)
    {
        if (m_shapes[i]->ischoose)
        {
            paint.setPen(m_shapes[i]->p);
            m_shapes[i]->rotate(thita);
            m_shapes[i]->DrawIt(&paint,m_shapes[i]->p.color());
        }
        qDebug() << "画转了"<<endl;
    }
}

void PaintArea::paintZoom(double x,double y,QImage &image)
{
    QPainter paint(&image);
    for(int i=0;i<m_shapes.size();i++)
    {
        if (m_shapes[i]->ischoose)
        {
            paint.setPen(m_shapes[i]->p);
            m_shapes[i]->zoom(x,y);
            m_shapes[i]->DrawIt(&paint,m_shapes[i]->p.color());
        }
    }
}


//画画，进入具体画形函数的上一步
void PaintArea::paintImage(QImage &image)
{

     if (m_drawShapeType == EnumDrawEllipse|m_drawShapeType == EnumDrawCircle|m_drawShapeType == EnumDrawLinecenter | m_drawShapeType == EnumDrawLineBresenham | m_drawShapeType == EnumDrawRect|m_drawShapeType == EnumDrawRect) {

        m_currShapes->setEndPoint(m_endPoint/m_scale);
        QPainter paint(&image);
        m_currShapes->DrawIt(&paint,pen.color());
        m_isModify = true;
    }
    else if (m_drawShapeType == EnumDrawPolygon) {

         paintPolygon(pointList,pen,image);
    }
    else if (m_drawShapeType == EnumScanFill) {
        fillbyScan(m_beginPoint, m_endPoint, image);
    }
    else if (m_drawShapeType == EnumSeedFill) {
         QColor color=image.pixel(m_beginPoint);
        fillbySeed(m_beginPoint, pen.color(), color, image);
    }
    else if(m_drawShapeType == EnumCohenSutherland | m_drawShapeType == EnumCutLineCenter)
    {
         m_assistShapes->setEndPoint(m_endPoint/m_scale);
         QPainter paint(&m_tmpImage);
         m_assistShapes->DrawIt(&paint,QColor(255,0,0));
         m_isModify = true;
    }
     else if (m_drawShapeType == EnumTransShape) {
         QPainter paint(&image);
         paintTrans(&paint,m_shapes);  //移动选中的
     }
     else if (m_drawShapeType == EnumZoomShape) {
         paintZoom(sX,sY,image);
     }
     else if (m_drawShapeType == EnumRotateShape) {
         paintRotate(thita,image);
     }
     else if (m_drawShapeType == EnumDrawBeizer) {
         QPainter paint(&image);

         if (bezierTrigger == false)   //是否产生曲线
         {
             if (fixpointTrigger == false)   //没有控制点被选中
             {
                 for (int k=0;k<m_assistShapes2.size();k++)
                 {
                     m_assistShapes2[k]->DrawIt(&paint,pen.color());
                 }
             }
             else             //控制点被选中
                 {
                  paintTrans(&paint,m_assistShapes2);
             }

         }
         else {
             m_currShapes->DrawIt(&paint,pen.color());
             bezierTrigger = false;
             fixpointTrigger = false;
         }
         m_isModify = true;
     }

    update();
}


void PaintArea::paintEvent(QPaintEvent */*event*/)
{
    QPainter paint(this);
    paint.scale(m_scale, m_scale);

    if (m_isDraw) {
      paint.drawImage(0, 0, m_tmpImage);
    }
    else {

        if (m_drawShapeType == EnumTransShape | m_drawShapeType == EnumZoomShape | m_drawShapeType == EnumRotateShape | (m_drawShapeType == EnumDrawBeizer&& bezierTrigger == false&& fixpointTrigger == true ) ) m_image=m_tmpImage;  //平移模式移动好以后，覆盖主画布

        paint.drawImage(0, 0, m_image);
    }
}



void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if (m_drawShapeType == EnumZoomShape | m_drawShapeType == EnumRotateShape) return;

    if (event->button() == Qt::RightButton) {
        if(m_drawShapeType == EnumDrawPolygon)
        {
            m_isDraw = false;
            m_movePoint = event->pos();
            //将第一个点传给容器，为了让最后一个点与第一个点在双击之后进行相连

            pointList.push_back(pointList[0] / m_scale);

            polygon *polygon2 = new polygon;
            polygon2->pointList=pointList;
            polygon2->polygonInner = new polygonInner();
            polygon2->polygonInner->points = pointList;
            polygon2->polygonInner->showPolygonInEdgeTable();

            polygon2->p=pen;
            polygon2->width=width;
            polygon2->setcenter();
            m_shapes.push_back(polygon2);

            paintImage(m_image);

            pointList.clear();//清空，方便下一次画
        }
        if(m_drawShapeType == EnumDrawBeizer)
        {
            m_isDraw = false;

            for (int k=0;k<m_assistShapes2.size();k++)
            {
                pointList.push_back(m_assistShapes2[k]->getBeginPoint());
            }

            Bezier *Bezier2 = new Bezier;
            Bezier2->fixPoints = pointList;

            qDebug() << pointList <<endl;
            Bezier2->width = width;
            Bezier2->p = pen;
             Bezier2->setcenter();
            m_shapes.push_back(Bezier2);

            m_currShapes=Bezier2;

            bezierTrigger = true;

            paintImage(m_image);

            pointList.clear();

            QVector<Shape *>().swap(m_assistShapes2);
            //清空，方便下一次画
        }
    }


    if (event->button() == Qt::LeftButton) {

        if(m_isChoose){//处于选择状态，左键进行选择
            m_choosePoint = event->pos();
            chooseshape(m_choosePoint);

        }

        else{
            m_beginPoint = event->pos();
            m_endPoint = event->pos();
            m_isDraw = true;

           if(m_drawShapeType == EnumDrawPolygon)
            {

               pointList.push_back(m_beginPoint / m_scale);

            }
           else if(m_drawShapeType == EnumDrawBeizer)
            {
               //这么个逻辑，点击了圆圈则移动圆圈，否则添加圆圈

               cleanFixPoint(event->pos()/m_scale);

               //画布逻辑和平移类似,保存非选中

            }
           else if(m_drawShapeType == EnumTransShape)
            {
               m_tmpImageTrans = QImage(1500, 750, QImage::Format_RGB32);
               m_imageBgColor = QColor(255, 255, 255);
               m_tmpImageTrans.fill(m_imageBgColor);

               QPainter painter(&m_tmpImageTrans); //先将m_tmpImageTrans画上非选中图元以备用
               for(int i=0;i<m_shapes.size();i++)
               {
                   if(!m_shapes[i]->ischoose)
                   {
                       m_shapes[i]->DrawIt(&painter,m_shapes[i]->p.color());
                   }
               }

            }
           else if (m_drawShapeType == EnumDrawLinecenter) {
               Linecenter *line = new Linecenter;
               line->beginPoint = m_beginPoint/m_scale;
               line->endPoint = m_endPoint/m_scale;
               line->ischoose = false;
               line->p=pen;
               line->width=width;
               line->m_drawLineType=m_drawLineType;
               line->setcenter();

               m_currShapes=line;

           }
           else if (m_drawShapeType == EnumDrawLineBresenham) {
               LineBresenham *line = new LineBresenham;
               line->beginPoint = m_beginPoint/m_scale;
               line->endPoint = m_endPoint/m_scale;
               line->ischoose = false;
               line->p=pen;
               line->m_drawLineType=m_drawLineType;
               line->width=width;
               line->setcenter();


               m_currShapes=line;

           }
           else if (m_drawShapeType == EnumDrawRect) {

               Rect *rect = new Rect;
               rect->beginPoint = m_beginPoint/m_scale;
               rect->endPoint = m_endPoint/m_scale;
               rect->ischoose = false;
               rect->p=pen;
               rect->width=width;
               rect->m_drawLineType=m_drawLineType;
               rect->setcenter();


               m_currShapes=rect;

           }
           else if (m_drawShapeType == EnumDrawCircle) {
               Circle *circle = new Circle;
               circle->beginPoint = m_beginPoint/m_scale;
               circle->endPoint = m_endPoint/m_scale;
               circle->ischoose = false;
               circle->p=pen;
               circle->width=width;
               circle->m_drawLineType=m_drawLineType;
               circle->setcenter();


               m_currShapes=circle;

           }
           else if (m_drawShapeType == EnumDrawEllipse) {
               ellipse *ellipse2 = new ellipse;
               ellipse2->beginPoint = m_beginPoint/m_scale;
               ellipse2->endPoint = m_endPoint/m_scale;
               ellipse2->ischoose = false;
               ellipse2->p=pen;
               ellipse2->width=width;
               ellipse2->m_drawLineType=m_drawLineType;
               ellipse2->setcenter();


               m_currShapes=ellipse2;

           }
           else if (m_drawShapeType == EnumCohenSutherland) {
                Rect *rect = new Rect;
                rect->beginPoint = m_beginPoint/m_scale;
                rect->endPoint = m_endPoint/m_scale;
                rect->ischoose = false;
                rect->p=pen;
                rect->width=width;
                rect->m_drawLineType=m_drawLineType;
                rect->setcenter();

                m_assistShapes=rect;

           }
           else if (m_drawShapeType == EnumCutLineCenter) {
                Rect *rect = new Rect;
                rect->beginPoint = m_beginPoint/m_scale;
                rect->endPoint = m_endPoint/m_scale;
                rect->ischoose = false;
                rect->width=width;
                rect->m_drawLineType=m_drawLineType;
                rect->width=width;
                rect->m_drawLineType=m_drawLineType;
                rect->setcenter();

                m_assistShapes=rect;
           }

        }
    }
}

void PaintArea::cleanFixPoint(QPoint chosepoint)  //控制bezier控制点
{
    if (m_assistShapes2.size() == 0)
    {
        Circle *circle = new Circle;
        circle->beginPoint = m_beginPoint/m_scale;
        circle->endPoint = (m_beginPoint/m_scale)+QPoint(4,4);
        circle->ischoose = false;
        circle->p=pen;
        circle->width=2;
        circle->m_drawLineType=Shape::defaultLine;
        circle->setcenter();
        m_assistShapes2.push_back(circle);

        return;
    }

    for (int k=0;k<m_assistShapes2.size();k++)   //有点可选时
    {
        if(m_assistShapes2[k]->IsIn(chosepoint))
         {
                m_assistShapes2[k]->ischoose=true;
                fixpointTrigger = true;

                m_tmpImageTrans = QImage(1500, 750, QImage::Format_RGB32);
                m_imageBgColor = QColor(255, 255, 255);
                m_tmpImageTrans.fill(m_imageBgColor);

                QPainter painter(&m_tmpImageTrans);  //先将m_tmpImageTrans画上非选中图元以备用
                for(int i=0;i<m_assistShapes2.size();i++)  //画上现有控制点，和图元
                {
                    if(!m_assistShapes2[i]->ischoose)
                    {
                        m_assistShapes2[i]->DrawIt(&painter,m_assistShapes2[i]->p.color());
                    }
                }
                for(int i=0;i<m_shapes.size();i++)
                {
                    if(!m_shapes[i]->ischoose)
                    {
                        m_shapes[i]->DrawIt(&painter,m_shapes[i]->p.color());
                    }
                }

                return ;
          }

    }

    fixpointTrigger = false;
    Circle *circle = new Circle;
    circle->beginPoint = m_beginPoint/m_scale;
    circle->endPoint = (m_beginPoint/m_scale)+QPoint(4,4);
    circle->ischoose = false;
    circle->p=pen;
    circle->width=2;
    circle->m_drawLineType=Shape::defaultLine;
    circle->setcenter();
    m_assistShapes2.push_back(circle);

}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drawShapeType == EnumZoomShape | m_drawShapeType == EnumRotateShape | m_isChoose ) return;

    if (event->buttons() & Qt::LeftButton)
    {
            if (m_drawShapeType == EnumDrawPolygon)
                    {
                         m_isMove = true;
                    }
            else if (m_drawShapeType == EnumTransShape | (m_drawShapeType == EnumDrawBeizer&& bezierTrigger == false&& fixpointTrigger == true )  ) {

                deltaPoint = m_endPoint;
                m_endPoint = event->pos();

                deltaPoint = m_endPoint -deltaPoint;
                m_tmpImage = m_tmpImageTrans;
                paintImage(m_tmpImage);
            }

           else
           {

                   m_endPoint = event->pos();
                   m_tmpImage = m_image;
                   paintImage(m_tmpImage); //当鼠标进行移动时，不断用之前的图片进行操作，防止不断地重绘
           }


       }

    }


void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_drawShapeType == EnumZoomShape | m_drawShapeType == EnumRotateShape | m_isChoose ) return;

    if (event->button() == Qt::LeftButton)
    {
        m_endPoint = event->pos();
        m_isDraw = false;

         if (m_drawShapeType == EnumDrawLinecenter | m_drawShapeType == EnumDrawLineBresenham) {

            m_currShapes->setEndPoint(m_endPoint/m_scale);
            m_shapes.push_back(m_currShapes);

            struct Line ln;
            ln.beginPoint=m_beginPoint;
            ln.endPoint=m_endPoint;
            ll1.push_back(ln);
        }
        else if (m_drawShapeType == EnumDrawRect | m_drawShapeType == EnumDrawCircle|m_drawShapeType == EnumDrawEllipse) {

             m_currShapes->setEndPoint(m_endPoint/m_scale);
             m_shapes.push_back(m_currShapes);

        }
        else if (m_drawShapeType == EnumDrawPolygon) {

            m_isMove = false;

        }
        else if (m_drawShapeType == EnumCohenSutherland) {
           // clear();
            float xl =  m_beginPoint.x();
            float xr = m_endPoint.x();
            float yb = m_beginPoint.y();
            float yt = m_endPoint.y();
            float x1,y1,x2,y2;
            float x=xl,y=yb;
            cout<<xl<<' '<<xr<<"  "<<ll1.size();
            xl=min(xl,xr);
            xr=max(x,xr);
            yb=min(yb,yt);
            yt=max(y,yt);

            for(int i=0;i<ll1.size();i++){
                x1=ll1[i].beginPoint.x();
                x2=ll1[i].endPoint.x();
                y1=ll1[i].beginPoint.y();
                y2=ll1[i].endPoint.y();
                CohenSutherland(x1,y1,x2,y2,xl,yb,xr,yt,m_image);
            }

        }
        else if (m_drawShapeType == EnumCutLineCenter) {

            float xl =  m_beginPoint.x();
            float xr = m_endPoint.x();
            float yb = m_beginPoint.y();
            float yt = m_endPoint.y();
            float x1,y1,x2,y2;
            float x=xl,y=yb;
            cout<<xl<<' '<<xr<<"  "<<ll1.size();
            xl=min(xl,xr);
            xr=max(x,xr);
            yb=min(yb,yt);
            yt=max(y,yt);

            for(int i=0;i<ll1.size();i++){
                x1=ll1[i].beginPoint.x();
                x2=ll1[i].endPoint.x();
                y1=ll1[i].beginPoint.y();
                y2=ll1[i].endPoint.y();
                Middleland(x1,y1,x2,y2,xl,yb,xr,yt,m_image);
            }
        }
        else if (m_drawShapeType == EnumDrawBeizer)
         {
             for (int k=0;k<m_assistShapes2.size();k++)
             {
                 if (m_assistShapes2[k]->ischoose)   //移动结束
                 {
                     m_assistShapes2[k]->ischoose = false;
                 }

             }
         }
         paintImage(m_image);
    }
}



//键盘按下事件
void PaintArea::keyPressEvent(QKeyEvent *ev)
{
    qDebug()   << "键盘" <<endl;
    m_tmpImage = m_tmpImageTrans;
    if(ev->key() == Qt::Key_W)
    {
        if(m_drawShapeType == PaintArea::EnumRotateShape)
        {
         qDebug()  << "按了旋转了" <<endl;
            thita = 1;

            paintImage(m_tmpImage);

        }
        else if(m_drawShapeType == PaintArea::EnumZoomShape)
        {

        //     << "按了缩放了" <<endl;
            sX=1.1;
            sY=1.1;
            paintImage(m_tmpImage);
        }
    }
    if(ev->key() == Qt::Key_S)
    {
        if(m_drawShapeType == PaintArea::EnumRotateShape)
        {

        qDebug()   << "按了旋转了" <<endl;
            thita = -1;
            paintImage(m_tmpImage);

        }
        else if(m_drawShapeType == PaintArea::EnumZoomShape)
        {
        //     << "按了缩放了" <<endl;
            sX=0.9;
            sY=0.9;
            paintImage(m_tmpImage);

        }
    }
  }
