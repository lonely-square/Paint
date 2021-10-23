#include "shape.h"


#include <QDebug>

Eigen::Vector3d pointToVecter(QPoint p)
{
    Eigen::Vector3d vecter= Eigen::Vector3d(p.x(),p.y(),1);
    return vecter;
}

//获得2维的平移矩阵
Eigen::Matrix3d getTranslation2d(int x,int y)
{
    Eigen::Matrix3d translationMatrix =Eigen::Matrix3d::Identity();
    translationMatrix(0,2) = x;
    translationMatrix(1,2) = y;
    return translationMatrix;
}

//获得2维的旋转矩阵,基于原点
Eigen::Matrix3d getRotate2d(double thita)
{
    Eigen::Matrix3d rotateMatrix =Eigen::Matrix3d::Identity();
    double cos1 = cos(thita);
    double sin1 = sin(thita);
    rotateMatrix(0,0) = cos1;
    rotateMatrix(0,1) = -sin1;
    rotateMatrix(1,1) = cos1;
    rotateMatrix(1,0) = sin1;
    return rotateMatrix;
}

//获得2维的放缩矩阵，基于原点
Eigen::Matrix3d getScale2d(double x,double y)
{
    Eigen::Matrix3d scaleMatrix =Eigen::Matrix3d::Identity();
    scaleMatrix(0,0) = x;
    scaleMatrix(1,1) = y;
    return scaleMatrix;
}


//获得2维的旋转矩阵,基于任意点
Eigen::Matrix3d getRotate2dCenter(double thita,QPoint center)
{
    int x=center.x(),y=center.y();
    Eigen::Matrix3d rotateCenterMatrix =Eigen::Matrix3d::Identity();
    Eigen::Matrix3d Matrix1 = getTranslation2d(-x,-y);
    Eigen::Matrix3d Matrix2 = getRotate2d(thita);
    cout<< Matrix2 <<endl;
    Eigen::Matrix3d Matrix3 = getTranslation2d(x,y);
    rotateCenterMatrix= Matrix3*Matrix2*Matrix1*rotateCenterMatrix;
    return rotateCenterMatrix;
}

//获得2维的放缩矩阵,基于任意点
Eigen::Matrix3d getScale2dCenter(double x,double y,QPoint center)
{
    int x2=center.x(),y2=center.y();
    Eigen::Matrix3d scaleCenterMatrix =Eigen::Matrix3d::Identity();
    Eigen::Matrix3d Matrix1 = getTranslation2d(-x2,-y2);

    Eigen::Matrix3d Matrix2 = getScale2d(x,y);

    Eigen::Matrix3d Matrix3 = getTranslation2d(x2,y2);

    scaleCenterMatrix= Matrix3*Matrix2*Matrix1*scaleCenterMatrix;

    return scaleCenterMatrix;
}


//下面都是基础画画方法
void Linecenter::paintLinecenter(QPainter* paint)
{
    pencil->width = width;
    cout << width <<endl;
    paint->setPen(p);

    if (m_drawLineType == dashLine)
    {
        int x0 = beginPoint.x();
        int y0 = beginPoint.y();
        int x1 = endPoint.x();
        int y1 = endPoint.y();
        int x = x0, y = y0;
        int a = y0 - y1, b = x1 - x0;
        int cx = (b >= 0 ? 1 : (b = -b, -1));
        int cy = (a <= 0 ? 1 : (a = -a, -1));

        int brokenFlag=0;

        if ( brokenFlag%(width*6) >width*0 && brokenFlag%(width*6) < width*4 ) pencil->CirclePen(paint,x,y);

        int d, d1, d2;
        if (-a <= b)		// 斜率绝对值 <= 1
        {
            d = 2 * a + b;
            d1 = 2 * a;
            d2 = 2 * (a + b);
            while(x != x1)
            {
                if (d < 0)
                    y += cy, d += d2;
                else
                    d += d1;
                x += cx;

                brokenFlag++;
                if ( brokenFlag%(width*6) >0 && brokenFlag%(width*6) < width*4 )
                pencil->CirclePen(paint,x,y);


            }
        }
        else				// 斜率绝对值 > 1
        {
            d = 2 * b + a;
            d1 = 2 * b;
            d2 = 2 * (a + b);
            while(y != y1)
            {
                if(d < 0)
                    d += d1;
                else
                    x += cx, d += d2;
                y += cy;

                brokenFlag++;
               if ( brokenFlag%(width*6) >0 && brokenFlag%(width*6) < width*4 )
                   pencil->CirclePen(paint,x,y);

            }
        }

    }
    else if (m_drawLineType == defaultLine) {

        {
            int x0 = beginPoint.x();
            int y0 = beginPoint.y();
            int x1 = endPoint.x();
            int y1 = endPoint.y();
            int x = x0, y = y0;
            int a = y0 - y1, b = x1 - x0;
            int cx = (b >= 0 ? 1 : (b = -b, -1));
            int cy = (a <= 0 ? 1 : (a = -a, -1));

            int brokenFlag=0;


            pencil->CirclePen(paint,x,y);

            int d, d1, d2;
            if (-a <= b)		// 斜率绝对值 <= 1
            {
                d = 2 * a + b;
                d1 = 2 * a;
                d2 = 2 * (a + b);
                while(x != x1)
                {
                    if (d < 0)
                        y += cy, d += d2;
                    else
                        d += d1;
                    x += cx;

                    brokenFlag++;


                   pencil->CirclePen(paint,x,y);


                }
            }
            else				// 斜率绝对值 > 1
            {
                d = 2 * b + a;
                d1 = 2 * b;
                d2 = 2 * (a + b);
                while(y != y1)
                {
                    if(d < 0)
                        d += d1;
                    else
                        x += cx, d += d2;
                    y += cy;

                    brokenFlag++;

                       pencil->CirclePen(paint,x,y);

                }
            }

        }

    }
}

void Rect::paintLineBresenham(QPoint beginPoint, QPoint endPoint,QPen p,int width, QPainter* paint)
{

    pencil->width=width;
    paint->setPen(p);

    if (m_drawLineType == dashLine)
        {
        int x0 = beginPoint.x();
        int y0 = beginPoint.y();
        int x1 = endPoint.x();
        int y1 = endPoint.y();

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
                pencil->CirclePen(paint,x,y);
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
                pencil->CirclePen(paint,x,y);
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
    else if  (m_drawLineType == defaultLine)
    {
    int x0 = beginPoint.x();
    int y0 = beginPoint.y();
    int x1 = endPoint.x();
    int y1 = endPoint.y();

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
            pencil->CirclePen(paint,x,y);
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

            pencil->CirclePen(paint,x,y);
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

}

void polygon::paintLineBresenham(QPoint beginPoint, QPoint endPoint,QPen p,int width, QPainter* paint)
{

    pencil->width=width;
    paint->setPen(p);

    if (m_drawLineType == dashLine)
        {
        int x0 = beginPoint.x();
        int y0 = beginPoint.y();
        int x1 = endPoint.x();
        int y1 = endPoint.y();

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
                pencil->CirclePen(paint,x,y);
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
                pencil->CirclePen(paint,x,y);
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
    else if  (m_drawLineType == defaultLine)
    {
    int x0 = beginPoint.x();
    int y0 = beginPoint.y();
    int x1 = endPoint.x();
    int y1 = endPoint.y();

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
            pencil->CirclePen(paint,x,y);
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

            pencil->CirclePen(paint,x,y);
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

}

void LineBresenham::paintLineBresenham(QPainter* paint)
{

    pencil->width = width;

    paint->setPen(p);

    if (m_drawLineType == dashLine)
        {
        int x0 = beginPoint.x();
        int y0 = beginPoint.y();
        int x1 = endPoint.x();
        int y1 = endPoint.y();

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
                pencil->CirclePen(paint,x,y);
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
                pencil->CirclePen(paint,x,y);
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
    else if  (m_drawLineType == defaultLine)
    {
    int x0 = beginPoint.x();
    int y0 = beginPoint.y();
    int x1 = endPoint.x();
    int y1 = endPoint.y();

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
            pencil->CirclePen(paint,x,y);
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

            pencil->CirclePen(paint,x,y);
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

}

void Rect::paintRect(QPoint beginPoint, QPoint endPoint,QPen p,int width, QPainter* ptr)
{
    Rect::paintLineBresenham(beginPoint,QPoint(endPoint.x(),beginPoint.y()),p,width,ptr);
    Rect::paintLineBresenham(beginPoint,QPoint(beginPoint.x(),endPoint.y()),p,width,ptr);
    Rect::paintLineBresenham(endPoint,QPoint(beginPoint.x(),endPoint.y()),p,width,ptr);
    Rect::paintLineBresenham(endPoint,QPoint(endPoint.x(),beginPoint.y()),p,width,ptr);

}

void Circle::paintCircle(QPoint beginPoint, QPoint endPoint,QPen p,int width, QPainter* paint)
{
    pencil->width = width;

    paint->setPen(p);


    int x, y, w,h, xc,yc;
    double e,r;

    x = beginPoint.x();
    y = beginPoint.y();
    w = endPoint.x();
    h = endPoint.y();
    r= sqrt((w-x)*(w-x)+(h-y)*(h-y));
    xc=x;yc=y;
    x=0;y=r;
    e=1-r;

    if (m_drawLineType == Shape::defaultLine)
    {
        pencil->CirclePen(paint,x+xc,y+yc);
        pencil->CirclePen(paint,-x+xc,-y+yc);
        pencil->CirclePen(paint,y+xc,x+yc);
        pencil->CirclePen(paint,-y+xc,-x+yc);
        pencil->CirclePen(paint,x+xc,-y+yc);
        pencil->CirclePen(paint,-x+xc,y+yc);
        pencil->CirclePen(paint,-y+xc,x+yc);
        pencil->CirclePen(paint,y+xc,-x+yc);
        while(x<=y)
         {     if(e<0)   e+=2*x+3;
                  else   { e+=2*(x-y)+5; y--;}
                  x++;
                  pencil->CirclePen(paint,x+xc,y+yc);
                  pencil->CirclePen(paint,-x+xc,-y+yc);
                  pencil->CirclePen(paint,y+xc,x+yc);
                  pencil->CirclePen(paint,-y+xc,-x+yc);
                  pencil->CirclePen(paint,x+xc,-y+yc);
                  pencil->CirclePen(paint,-x+xc,y+yc);
                  pencil->CirclePen(paint,-y+xc,x+yc);
                  pencil->CirclePen(paint,y+xc,-x+yc);
                       // 画八分对称性的其他点
          }
    }
    else if (m_drawLineType == Shape::dashLine) {
        int brokenFlag = 0;


        if ( brokenFlag%(width*6) >width*0 && brokenFlag%(width*6) < width*4 )
        {
        pencil->CirclePen(paint,x+xc,y+yc);
        pencil->CirclePen(paint,-x+xc,-y+yc);
        pencil->CirclePen(paint,y+xc,x+yc);
        pencil->CirclePen(paint,-y+xc,-x+yc);
        pencil->CirclePen(paint,x+xc,-y+yc);
        pencil->CirclePen(paint,-x+xc,y+yc);
        pencil->CirclePen(paint,-y+xc,x+yc);
        pencil->CirclePen(paint,y+xc,-x+yc);
        }

        while(x<=y)
         {     if(e<0)   e+=2*x+3;
                  else   { e+=2*(x-y)+5; y--;}
                  x++;
                  brokenFlag++;
                  if ( brokenFlag%(width*6) >width*0 && brokenFlag%(width*6) < width*4 )
                  {
                      pencil->CirclePen(paint,x+xc,y+yc);
                      pencil->CirclePen(paint,-x+xc,-y+yc);
                      pencil->CirclePen(paint,y+xc,x+yc);
                      pencil->CirclePen(paint,-y+xc,-x+yc);
                      pencil->CirclePen(paint,x+xc,-y+yc);
                      pencil->CirclePen(paint,-x+xc,y+yc);
                      pencil->CirclePen(paint,-y+xc,x+yc);
                      pencil->CirclePen(paint,y+xc,-x+yc);
                  }

                       // 画八分对称性的其他点
          }
    }

}

void ellipse::paintEllipse(QPoint beginPoint, QPoint endPoint,QPen p,int width, QPainter* paint)
{
    pencil->width = width;

    paint->setPen(p);

        int x, y, w, h,a,b,xc,yc;
        float d1,d2;
        x = beginPoint.x() ;
        y = beginPoint.y() ;
        w = endPoint.x() ;
        h = endPoint.y() ;
        xc=x;yc=y;
        if(x>w) a=x-w;
        else a=w-x;
        if(y>h)b=y-h;
        else b=h-y;
        x=0;y=b;
        d1 = b * b + a * a * (-b + 0.25);

        if (m_drawLineType == Shape::defaultLine)
        {

        pencil->CirclePen(paint,x+xc,y+yc);
        pencil->CirclePen(paint,x-xc,y+yc);
        pencil->CirclePen(paint,x+xc,y-yc);
        pencil->CirclePen(paint,x-xc,y-yc);

        while (b * b * (x + 1) < a * a * (y - 0.5))
        {
            if (d1 < 0)
            {
                d1 += b * b * (2 * x + 3);
                x++;
            }
            else
            {
                d1 += (b * b * (2 * x + 3) + a * a * (-2 * y + 2));
                x++;
                y--;
            }


            pencil->CirclePen(paint,x+xc,y+yc);
            pencil->CirclePen(paint,x+xc,-y+yc);
            pencil->CirclePen(paint,-x+xc,y+yc);
            pencil->CirclePen(paint,-x+xc,-y+yc);


        }
        d2 = sqrt(b * (x + 0.5)) + sqrt(a * (y - 1)) - sqrt(a * b);
        while (y > 0)
        {
            if (d2 < 0)
            {
                d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
                x++;
                y--;
            }
            else
            {
                d2 += a * a * (-2 * y + 3);
                y--;
            }

            pencil->CirclePen(paint,x+xc,y+yc);
            pencil->CirclePen(paint,x+xc,-y+yc);
            pencil->CirclePen(paint,-x+xc,y+yc);
            pencil->CirclePen(paint,-x+xc,-y+yc);
        }
       }
        else if (m_drawLineType == Shape::dashLine) {
            int brokenFlag = 0;
            if ( brokenFlag%(width*6) >width*0 && brokenFlag%(width*6) < width*4 )
            {
                pencil->CirclePen(paint,x+xc,y+yc);
                pencil->CirclePen(paint,x-xc,y+yc);
                pencil->CirclePen(paint,x+xc,y-yc);
                pencil->CirclePen(paint,x-xc,y-yc);

            }

            while (b * b * (x + 1) < a * a * (y - 0.5))
            {
                if (d1 < 0)
                {
                    d1 += b * b * (2 * x + 3);
                    x++;
                }
                else
                {
                    d1 += (b * b * (2 * x + 3) + a * a * (-2 * y + 2));
                    x++;
                    y--;
                }
                brokenFlag++;
                if ( brokenFlag%(width*6) >width*0 && brokenFlag%(width*6) < width*4 )
                {
                pencil->CirclePen(paint,x+xc,y+yc);
                pencil->CirclePen(paint,x+xc,-y+yc);
                pencil->CirclePen(paint,-x+xc,y+yc);
                pencil->CirclePen(paint,-x+xc,-y+yc);
                }

            }
            d2 = sqrt(b * (x + 0.5)) + sqrt(a * (y - 1)) - sqrt(a * b);
            while (y > 0)
            {
                if (d2 < 0)
                {
                    d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
                    x++;
                    y--;
                }
                else
                {
                    d2 += a * a * (-2 * y + 3);
                    y--;
                }

                brokenFlag++;
                if ( brokenFlag%(width*6) >width*0 && brokenFlag%(width*6) < width*4 )
                {
                pencil->CirclePen(paint,x+xc,y+yc);
                pencil->CirclePen(paint,x+xc,-y+yc);
                pencil->CirclePen(paint,-x+xc,y+yc);
                pencil->CirclePen(paint,-x+xc,-y+yc);
                }
            }

          }

}



void Shape::translation(int offsetX,int offsetY)
{
    cout<<"没写好捏"<<endl;
}

void Shape::rotate(double thita)
{
    cout<<"没写好捏"<<endl;
}
void Shape::zoom(double x,double y)
{
    cout<<"没写好捏"<<endl;
}

void polygon::rotate(double thita)
{
    Eigen::Matrix3d matrix = getRotate2dCenter(thita,center);


    for (int i=0;i<pointList.size();i++)
    {
        Eigen::Vector3d vecter = pointToVecter(pointList[i]);

        Eigen::Vector3d res = matrix*vecter;

        pointList[i]=QPoint(res.x(),res.y());
    }
    polygonInner->points = pointList;
    polygonInner->showPolygonInEdgeTable();
    setcenter();

}

void polygon::zoom(double x,double y)
{
    Eigen::Matrix3d matrix = getScale2dCenter(x,y,center);

    for (int i=0;i<pointList.size();i++)
    {
        Eigen::Vector3d vecter = pointToVecter(pointList[i]);
        Eigen::Vector3d res = matrix*vecter;
        pointList[i]=QPoint(res.x(),res.y());

    }
    polygonInner->points = pointList;
    polygonInner->showPolygonInEdgeTable();
    setcenter();
}

void polygon::translation(int offsetX,int offsetY)
{

    for (int i=0;i<pointList.size();i++)
    {
        int x=pointList[i].x()+offsetX;
        int y=pointList[i].y()+offsetY;
        pointList[i]=QPoint(x,y);

    }

    polygonInner->points = pointList;
    polygonInner->showPolygonInEdgeTable();
    setcenter();
}

void Bezier::rotate(double thita)
{
    Eigen::Matrix3d matrix = getRotate2dCenter(thita,center);


    for (int i=0;i<fixPoints.size();i++)
    {
        Eigen::Vector3d vecter = pointToVecter(fixPoints[i]);

        Eigen::Vector3d res = matrix*vecter;

        fixPoints[i]=QPoint(res.x(),res.y());
    }

    setcenter();

}

void Bezier::zoom(double x,double y)
{
    Eigen::Matrix3d matrix = getScale2dCenter(x,y,center);

    for (int i=0;i<fixPoints.size();i++)
    {
        Eigen::Vector3d vecter = pointToVecter(fixPoints[i]);
        Eigen::Vector3d res = matrix*vecter;
        fixPoints[i]=QPoint(res.x(),res.y());

    }

    setcenter();
}

void Bezier::translation(int offsetX,int offsetY)
{

    for (int i=0;i<fixPoints.size();i++)
    {
        int x=fixPoints[i].x()+offsetX;
        int y=fixPoints[i].y()+offsetY;
        fixPoints[i]=QPoint(x,y);

    }

    setcenter();
}

void Linecenter::rotate(double thita)
{
    Eigen::Matrix3d matrix = getRotate2dCenter(thita,center);


        Eigen::Vector3d vecter = pointToVecter(beginPoint);

        Eigen::Vector3d res = matrix*vecter;

        qDebug() <<beginPoint << endl;

        beginPoint=QPoint(res.x(),res.y());

         qDebug() <<beginPoint << endl;
         vecter = pointToVecter(endPoint);

        res = matrix*vecter;

        endPoint=QPoint(res.x(),res.y());

    setcenter();

}

void Linecenter::zoom(double x,double y)
{
    Eigen::Matrix3d matrix = getScale2dCenter(x,y,center);


        Eigen::Vector3d vecter = pointToVecter(beginPoint);

        Eigen::Vector3d res = matrix*vecter;

        beginPoint=QPoint(res.x(),res.y());

         vecter = pointToVecter(endPoint);

        res = matrix*vecter;

        endPoint=QPoint(res.x(),res.y());

    setcenter();
}

void Linecenter::translation(int offsetX,int offsetY)
{


        beginPoint+=QPoint(offsetX,offsetY);
        endPoint+=QPoint(offsetX,offsetY);
    setcenter();
}

void LineBresenham::rotate(double thita)
{
    Eigen::Matrix3d matrix = getRotate2dCenter(thita,center);


        Eigen::Vector3d vecter = pointToVecter(beginPoint);

        Eigen::Vector3d res = matrix*vecter;

        beginPoint=QPoint(res.x(),res.y());

         vecter = pointToVecter(endPoint);

        res = matrix*vecter;

        endPoint=QPoint(res.x(),res.y());

    setcenter();

}

void LineBresenham::zoom(double x,double y)
{
    Eigen::Matrix3d matrix = getScale2dCenter(x,y,center);


        Eigen::Vector3d vecter = pointToVecter(beginPoint);

        Eigen::Vector3d res = matrix*vecter;

        beginPoint=QPoint(res.x(),res.y());

         vecter = pointToVecter(endPoint);

        res = matrix*vecter;

        endPoint=QPoint(res.x(),res.y());

    setcenter();
}

void LineBresenham::translation(int offsetX,int offsetY)
{


        beginPoint+=QPoint(offsetX,offsetY);
        endPoint+=QPoint(offsetX,offsetY);
    setcenter();
}

void Rect::rotate(double thita)
{
    Eigen::Matrix3d matrix = getRotate2dCenter(thita,center);


        Eigen::Vector3d vecter = pointToVecter(beginPoint);

        Eigen::Vector3d res = matrix*vecter;

        beginPoint=QPoint(res.x(),res.y());

         vecter = pointToVecter(endPoint);

        res = matrix*vecter;

        endPoint=QPoint(res.x(),res.y());

    setcenter();

}

void Rect::zoom(double x,double y)
{
    Eigen::Matrix3d matrix = getScale2dCenter(x,y,center);


        Eigen::Vector3d vecter = pointToVecter(beginPoint);

        Eigen::Vector3d res = matrix*vecter;

        beginPoint=QPoint(res.x(),res.y());

         vecter = pointToVecter(endPoint);

        res = matrix*vecter;

        endPoint=QPoint(res.x(),res.y());

    setcenter();
}

void Rect::translation(int offsetX,int offsetY)
{


        beginPoint+=QPoint(offsetX,offsetY);
        endPoint+=QPoint(offsetX,offsetY);
    setcenter();
}


void Circle::rotate(double thita)
{
    Eigen::Matrix3d matrix = getRotate2dCenter(thita,center);


        Eigen::Vector3d vecter = pointToVecter(beginPoint);

        Eigen::Vector3d res = matrix*vecter;

        beginPoint=QPoint(res.x(),res.y());

         vecter = pointToVecter(endPoint);

        res = matrix*vecter;

        endPoint=QPoint(res.x(),res.y());

    setcenter();

}

void Circle::zoom(double x,double y)
{
    Eigen::Matrix3d matrix = getScale2dCenter(x,y,center);


        Eigen::Vector3d vecter = pointToVecter(beginPoint);

        Eigen::Vector3d res = matrix*vecter;

        beginPoint=QPoint(res.x(),res.y());

         vecter = pointToVecter(endPoint);

        res = matrix*vecter;

        endPoint=QPoint(res.x(),res.y());

    setcenter();
}

void Circle::translation(int offsetX,int offsetY)
{


        beginPoint+=QPoint(offsetX,offsetY);
        endPoint+=QPoint(offsetX,offsetY);
    setcenter();
}

void ellipse::rotate(double thita)
{
    Eigen::Matrix3d matrix = getRotate2dCenter(thita,center);


        Eigen::Vector3d vecter = pointToVecter(beginPoint);

        Eigen::Vector3d res = matrix*vecter;

        beginPoint=QPoint(res.x(),res.y());

         vecter = pointToVecter(endPoint);

        res = matrix*vecter;

        endPoint=QPoint(res.x(),res.y());

    setcenter();

}

void ellipse::zoom(double x,double y)
{
    Eigen::Matrix3d matrix = getScale2dCenter(x,y,center);


        Eigen::Vector3d vecter = pointToVecter(beginPoint);

        Eigen::Vector3d res = matrix*vecter;

        beginPoint=QPoint(res.x(),res.y());

         vecter = pointToVecter(endPoint);

        res = matrix*vecter;

        endPoint=QPoint(res.x(),res.y());

    setcenter();
}

void ellipse::translation(int offsetX,int offsetY)
{


        beginPoint+=QPoint(offsetX,offsetY);
        endPoint+=QPoint(offsetX,offsetY);
    setcenter();
}

bool polygon::IsIn(QPoint p)
{
    return polygonInner->isInPolygon(p);
}

void polygon::DrawIt(QPainter* ptr,QColor color)
{
    return polygonInner->drawPolyScans(ptr,color);
}

/*Bezier曲线生成*/
static double **P_x = nullptr; //中间计算过程需要用到的二维数组
static double **P_y = nullptr; //中间计算过程需要用到的二维数组

QPoint getPoint_Bezier(double u, const QVector<QPoint> &vertexs) {
    //初始化
    int n = vertexs.size();
    for (int i = 0; i < n; i++) {
        P_x[i][0] = vertexs[i].x();
        P_y[i][0] = vertexs[i].y();
    }
    //依次降阶计算（类似动态规划的计算过程）
    for (int r = 1; r < n; r++) {
        for (int i = 0; i < n - r; i++) {
            //P[i][r]=(1-u)*P[i][r-1]+u*P[i+1][r-1]; //递推式 - 关键
            P_x[i][r] = (1 - u)*P_x[i][r - 1] + u * P_x[i + 1][r - 1]; //注：此出处计算应该全程使用double类型，最终取点的时候再取整，否则误差很大，曲线不够光滑
            P_y[i][r] = (1 - u)*P_y[i][r - 1] + u * P_y[i + 1][r - 1];
        }
    }
    return QPoint(qRound(P_x[0][n - 1]), qRound(P_y[0][n - 1]));
}
//决策步长
double getStep(const QVector<QPoint> &vertexs) {
    double sum = 1;
    for (int i = 1; i < vertexs.size(); i++) {
        sum += sqrt((vertexs[i].x() - vertexs[i - 1].x())*(vertexs[i].x() - vertexs[i - 1].x()) + (vertexs[i].y() - vertexs[i - 1].y())*(vertexs[i].y() - vertexs[i - 1].y()));
    }
    return 1 / (sum*1.5);
}
//根据参数得出Bezier曲线
void drawCurve_Bezier(const QVector<QPoint> &vertexs, QVector<QPoint> &myset)
{
    int n = vertexs.size();
    //分配空间
    P_x = new double*[n];
    P_y = new double*[n];
    for (int i = 0; i < n; i++) {
        P_x[i] = new double[n];
        P_y[i] = new double[n];
    }

    //以一定步长生成曲线上的离散点
    double STEP = getStep(vertexs);
    for (double u = 0.0; u <= 1.0; u += STEP) {
        QPoint temp = getPoint_Bezier(u, vertexs);
        myset.push_back(temp);
    }
    //释放空间
    for (int i = 0; i < n; i++) {
        delete[] P_x[i];
        delete[] P_y[i];
    }
    delete[] P_x;
    delete[] P_y;
}

void Bezier::create()
{
    return drawCurve_Bezier(fixPoints,resPoints);
}

bool Bezier::IsIn(QPoint p)
{
    int minX=fixPoints[0].x(),minY=fixPoints[0].y(),maxX=fixPoints[0].x(),maxY=fixPoints[0].y();
    QVector<QPoint>::iterator iter;
    for (iter=fixPoints.begin(); iter!=fixPoints.end(); iter++)
    {
        if (iter->y() > maxY)
            maxY = iter->y();
    if (iter->y() < minY)
        minY = iter->y();
    if (iter->x() > maxX)
        maxX = iter->x();
    if (iter->x() < minX)
        minX = iter->x();
     }

    if( (p.x()>minX && p.x()<maxX)&&(p.y()>minY && p.y()<maxY))
    {
        cout<<"choose Bezier"<<endl;
        return true;
    }
    return false;
}

void Bezier::DrawIt(QPainter* ptr,QColor color)
{
    resPoints.clear();
    create();
    p.setColor(Qt::red);
    ptr->setPen(p);
    for (int i=0;i < resPoints.size() ; i++) {
        ptr->drawPoint(resPoints[i]);
    }
}

bool Linecenter::IsIn(QPoint p)
{
    qDebug()<< "choose zhixian" <<endl;
    int ymax,ymin;
       if(beginPoint.y()>endPoint.y()){
           ymax = beginPoint.y();
           ymin = endPoint.y();
       }
       else{
           ymin = beginPoint.y();
           ymax = endPoint.y();
       }
\

       if(p.y()<=ymax&&p.y()>=ymin) return false;

       double A=endPoint.y()-beginPoint.y();
       double B=-(endPoint.x()-beginPoint.x());
       double C=endPoint.x()*beginPoint.y()-endPoint.y()*beginPoint.x();
       double res=abs((A*p.x()+B*p.y()+C)/sqrt(A*A+B*B));
        qDebug()<< res <<endl;
       if(res<=width){
               qDebug()<< "choosed zhixian" <<endl;
               return true;
           }

     return false;

}

void Linecenter::DrawIt(QPainter* ptr,QColor color)
{
    return Linecenter::paintLinecenter(ptr);
}

bool LineBresenham::IsIn(QPoint p)
{
    qDebug()<< "choose zhixian" <<endl;
    int ymax,ymin;
       if(beginPoint.y()>endPoint.y()){
           ymax = beginPoint.y();
           ymin = endPoint.y();
       }
       else{
           ymin = beginPoint.y();
           ymax = endPoint.y();
       }

       if(p.y()<=ymax&&p.y()>=ymin){
           if((endPoint.y()-beginPoint.y())*p.x()-(endPoint.x()-beginPoint.x())*p.y()+(endPoint.x()-beginPoint.x())*beginPoint.y()-(endPoint.y()-beginPoint.y())*beginPoint.x()<=width){
               qDebug()<< "choosed zhixian" <<endl;
               return true;
           }
           else
               return false;
       }
       else
           return false;
}

void LineBresenham::DrawIt(QPainter* ptr,QColor color)
{
    return LineBresenham::paintLineBresenham(ptr);
}

bool Rect::IsIn(QPoint p)
{

    int ymax, ymin, xmax, xmin;
       if(beginPoint.y()>endPoint.y()){
           ymax = beginPoint.y();
           ymin = endPoint.y();
       }
       else{
           ymin = beginPoint.y();
           ymax = endPoint.y();
       }
       if(beginPoint.x()>endPoint.x()){
           xmax = beginPoint.x();
           xmin = endPoint.x();
       }
       else{
           xmin = beginPoint.x();
           xmax = endPoint.x();
       }

       if(p.y()<=ymax&&p.y()>=ymin&&p.x()<=xmax&&p.x()>=xmin&&width!=0)
           return true;
       else
           return false;

}


void Rect::DrawIt(QPainter* ptr,QColor color)
{
    return paintRect(beginPoint,endPoint,p,width,ptr);
}

bool Circle::IsIn(QPoint p)
{
    int a, b;
    p -= beginPoint;

         a = endPoint.x()-beginPoint.x();
         b = endPoint.y()-beginPoint.y();
         if(p.x()*p.x()+p.y()*p.y()<a*a+b*b)
         {
             cout <<"choose circle"<<endl;
             return true;}
         else
             return false;
}

void Circle::DrawIt(QPainter* ptr,QColor color)
{
    return paintCircle(beginPoint,endPoint,p,width,ptr);
}

bool ellipse::IsIn(QPoint p)
{
    int a, b;
    p -= beginPoint;

         a = endPoint.x()-beginPoint.x();
         b = endPoint.y()-beginPoint.y();
         if(p.x()*p.x()/(a*a)+p.y()*p.y()/(b*b)<1)
         {
             cout <<"choose ellipse"<<endl;
             return true;}
         else
             return false;

}

void ellipse::DrawIt(QPainter* ptr,QColor color)
{
    return paintEllipse(beginPoint,endPoint,p,width,ptr);
}

bool Shape::setEndPoint(QPoint)
{
    return false;
}

bool Linecenter::setEndPoint(QPoint p)
{
    endPoint = p;
    return true;
}

bool Rect::setEndPoint(QPoint p)
{
    endPoint = p;
    return true;
}

bool ellipse::setEndPoint(QPoint p)
{
    endPoint = p;
    return true;
}

bool Circle::setEndPoint(QPoint p)
{
    endPoint = p;
    return true;
}

bool LineBresenham::setEndPoint(QPoint p)
{
    endPoint = p;
    return true;
}

