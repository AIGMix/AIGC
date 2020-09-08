#pragma once
#include <string>

namespace aigc
{
//点
typedef struct PointStruct
{
    double x;
    double y;
}Point;

//边框
typedef struct RectStruct
{
    double left;
    double top;
    double right;
    double bottom;
}Rect;

//直线的一般方程式(Ax+By+C=0)
typedef struct LineEquationStruct
{
    double A;
    double B;
    double C;
}LineEquation;

class MathHelper
{
public:
    /**
    * @brief 是否为大端模式
    * @return 是否
    */
    static bool IsBigEndian();

    /**
    * @brief 获取四字节对齐的长度
    * @param size 原长度
    * @return 对齐后的长度
    */
    static int Calc4ByteAlignSize(int size);

    /**
    * @brief 是否为相同点
    * @param point1 点1
    * @param point2 点2
    * @return 是否
    */
    static bool IsSamePoint(const Point &point1, const Point &point2);

    /**
    * @brief 获取直线的一般方程式
    * @param point1 点1
    * @param point2 点2
    */
    static LineEquation GetLineEquation(const Point& point1, const Point& point2);

    /**
    * @brief 计算两点间距离
    * @param point1 点1
    * @param point2 点2
    * @return 距离
    */
    static double Calc2PointsDistance(const Point& point1, const Point& point2);

    /**
    * @brief 计算点到线的距离
    * @param point 点
    * @param linePoint1 线上的点1
    * @param linePoint2 线上的点2
    * @return 距离
    */
    static double CalcPointToLineDistance(const Point& point, const Point& linePoint1, const Point& linePoint2);

    /**
    * @brief 计算线与X轴的夹角
    * @param point1 点1
    * @param point2 点2
    * @return 角度
    */
    static double CalcLineToXAxisAngle(const Point &point1, const Point &point2);

    /**
    * @brief 判断点在线的那一侧
    * @param point 点
    * @param linePoint1 线上的点1
    * @param linePoint2 线上的点2
    * @return 0线上 1左侧 2右侧 -1错误
    */
    static int CalcPointToLineSide(const Point &point, const Point &linePoint1, const Point &linePoint2);

    /**
    * @brief 获取线段上的点
    * @param point1 点1
    * @param point2 点2
    * @param disPoint1 此点到点1的距离
    * @return 点
    */
    static Point GetPointFromSegment(const Point& point1, const Point& point2, double disPoint1);

    /**
    * @brief 获取点到线的垂足
    * @param point 点
    * @param linePoint1 线上的点1
    * @param linePoint2 线上的点2
    * @return 垂足
    */
    static Point GetVPointFromPointToLine(const Point &point, const Point &linePoint1, const Point &linePoint2);

};
}
