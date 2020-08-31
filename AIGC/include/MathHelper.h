#pragma once
#include <string>

namespace AIGC
{
    //点
    typedef struct _Point
    {
        double dX;
        double dY;
    }Point;

    //边框
    typedef struct _Rect
    {
        double dLeft;
        double dTop;
        double dRight;
        double dBottom;
    }Rect;

    //直线的一般方程式(Ax+By+C=0)
    typedef struct _LineEquation
    {
        double dA;
        double dB;
        double dC;
    }LineEquation;

    class MathHelper
    {
    public:
        /**
         * @brief 是否为大端模式
         * @return {bool} 是否
         */
        static bool IsBigEndian();

        /**
         * @brief 获取四字节对齐的长度
         * @param {int} iSize 原长度
         * @return {int} 对齐后的长度
         */
        static int Calc4ByteAlignSize(int iSize);



        /**
         * @brief 计算两点间距离
         * @param {Point} oPoint1 点1
         * @param {Point} oPoint2 点2
         * @return {double} 距离
         */
        static double Calc2PointsDistance(const Point& oPoint1, const Point& oPoint2);

        /**
         * @brief 计算点到线的距离
         * @param {Point} oPoint 点
         * @param {Point} oLinePoint1 线上的点1
         * @param {Point} oLinePoint2 线上的点2
         * @return {double} 距离
         */
        static double CalcPointToLineDistance(const Point& oPoint, const Point& oLinePoint1, const Point& oLinePoint2);



        /**
         * @brief 获取线段上的点
         * @param {Point} oPoint1 点1
         * @param {Point} oPoint2 点2
         * @param {double} dDisPoint1 此点到点1的距离
         * @return {Point} 点
         */
        static Point GetPointFromSegment(const Point& oPoint1, const Point& oPoint2, double dDisPoint1);
    };
}
