#ifndef PENCIL_H
#define PENCIL_H

#include <QPen>
#include <QPainter>

//该类只是封装了方法，笔啊，画布的存储还是在paintarea中
class Pencil
{
public:
    Pencil();
    void RectPen(QPainter *painter,QColor color,int width,int x, int y);
    void CirclePen(QPainter *painter,int x, int y);
    int width;

};

#endif // PENCIL_H
