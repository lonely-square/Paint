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

bool polygon::IsIn(QPoint p)
{
    return polygonInner->isInPolygon(p);
}

void polygon::DrawIt(QPainter* ptr,QColor color,qreal scale)
{
    return polygonInner->drawPolyScans(ptr,color,scale);
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
    return true;
}

void Bezier::DrawIt(QPainter* ptr,QColor color,qreal scale)
{
    p.setColor(Qt::red);
    ptr->setPen(p);
    for (int i=0;i < resPoints.size() ; i++) {
        ptr->drawPoint(resPoints[i]/scale);
    }
}

bool Linecenter::IsIn(QPoint p)
{
    return false;
}

void Linecenter::DrawIt(QPainter* ptr,QColor color,qreal scale)
{

}

bool LineBresenham::IsIn(QPoint p)
{
    return false;
}

void LineBresenham::DrawIt(QPainter* ptr,QColor color,qreal scale)
{

}

bool Rect::IsIn(QPoint p)
{
    return false;
}

void Rect::DrawIt(QPainter* ptr,QColor color,qreal scale)
{

}

bool Circle::IsIn(QPoint p)
{
    return false;
}

void Circle::DrawIt(QPainter* ptr,QColor color,qreal scale)
{

}

bool ellipse::IsIn(QPoint p)
{
    return false;
}

void ellipse::DrawIt(QPainter* ptr,QColor color,qreal scale)
{

}

