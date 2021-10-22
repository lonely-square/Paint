#include "pencil.h"

int Pencil::width = 1;

Pencil::Pencil(){
    width = 1;
}

void Pencil::RectPen(QPainter *painter, QColor color,int width,int x,int y)
{

    for (int i=0;i<width;i++)
    {
            painter->drawPoint(x-i,y);
            painter->drawPoint(x+i,y);
            painter->drawPoint(x,y-i);
            painter->drawPoint(x,y+i);
    }
}


void Pencil::CirclePen(QPainter *painter,int x,int y)
{

    int xc,yc,r,e;
    r= width;
    xc=x;yc=y;    //yc,xc为基础坐标
    x=0;y=r;
    e=1.25-r;
    int tempy=y;                       //用tempy纵向打印
    while(tempy>=0)
    {
        painter->drawPoint(x+xc,tempy+yc);
        painter->drawPoint(-x+xc,-tempy+yc);
        painter->drawPoint(tempy+xc,x+yc);
        painter->drawPoint(-tempy+xc,-x+yc);
        painter->drawPoint(x+xc,-tempy+yc);
        painter->drawPoint(-x+xc,tempy+yc);
        painter->drawPoint(-tempy+xc,x+yc);
        painter->drawPoint(tempy+xc,-x+yc);
        tempy--;
    }
        while(x<=y)
         {     if(e<0)   e+=2*x+3;
                  else   { e+=2*(x-y)+5; y--;}
                  x++;
                  tempy=y;
                  while(tempy>=0)
                  {
                      painter->drawPoint(x+xc,tempy+yc);
                      painter->drawPoint(-x+xc,-tempy+yc);
                      painter->drawPoint(tempy+xc,x+yc);
                      painter->drawPoint(-tempy+xc,-x+yc);
                      painter->drawPoint(x+xc,-tempy+yc);
                      painter->drawPoint(-x+xc,tempy+yc);
                      painter->drawPoint(-tempy+xc,x+yc);
                      painter->drawPoint(tempy+xc,-x+yc);
                      tempy--;
                  }
                       // 画八分对称性的其他点
          }


}



