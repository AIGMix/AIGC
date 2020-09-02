#include <math.h>
#include "MathHelper.h"

//圆周率
#define PI									 3.14159265359
//浮点数的边界
#define FLOAT_EQUAL_LIMIT					 0.0000001f	

//求绝对值
#define MATH_ABS(value)			         ((value) > 0 ? (value) : -(value))
//最大值
#define MATH_MAX(A, B)					 (A > B ? A : B)
//最小值
#define MATH_MIN(A, B)					 (A > B ? B : A)

//浮点数是否相同
#define MATH_FLOAT_EQUAL(A, B)			 (MATH_ABS(A - B) < FLOAT_EQUAL_LIMIT)
//点是否相同
#define MATH_POINT_EQUAL(AX, AY, BX, BY) (MATH_FLOAT_EQUAL(AX, BX) && MATH_FLOAT_EQUAL(AY, BY))

namespace aigc
{
bool MathHelper::IsBigEndian()
{
    /**
    * 大端：认为第一个字节是最高位字节
    * 小端：它认为第一个字节是最低位字节
    * x86系列CPU都是小端，PowerPC通常是大端，还有的CPU能通过跳线来设置CPU工作于大端还是小端模式。
    * 判断方法：设置一个int型为1，取第一个字节，如果是1则为小端
    */
    unsigned int num = 0xff;
    unsigned char *pTmp = (unsigned char *)&num;
    if (*pTmp == 0xff)
        return false;
    return true;
}

int MathHelper::Calc4ByteAlignSize(int size)
{
    /**
    * 四字节对齐的必要条件是：
    * 1、只能补齐不能减少，比如未对齐之前的长度为1，对齐后只能是4而不是0
    * 2、判断字节对齐的条件是对4求余，如果为0就是对齐
    * 
    * ((in_Size + 3) >> 2) << 2 
    * 加三之后，保证只能被补齐而不会减少。 右移除于4，余数后面的都没了；再左移乘于4，则对齐
    */
    return size < 0 ? 0 : ((size + 3) >> 2) << 2;  //或者(in_Size + 3 & ~3)
}

bool MathHelper::IsSamePoint(const Point &point1, const Point &point2)
{
    if (MATH_POINT_EQUAL(point1.x, point1.y, point2.x, point2.y))
        return true;
    return false;
}

LineEquation MathHelper::GetLineEquation(const Point &point1, const Point &point2)
{
    LineEquation ret = { 0,0,0, };
    if (MATH_POINT_EQUAL(point1.x, point1.y, point2.x, point2.y))
        return ret;

    ret.A = point2.y - point1.y;
    ret.B = point1.x - point2.x;
    ret.C = point2.x * point1.y - point1.x * point2.y;
    return ret;
}

double MathHelper::Calc2PointsDistance(const Point& point1, const Point& point2)
{
    if (MATH_POINT_EQUAL(point1.x, point1.y, point2.x, point2.y))
        return 0.0;

    return sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));
}

double MathHelper::CalcPointToLineDistance(const Point& point, const Point& linePoint1, const Point& linePoint2)
{
    if (MATH_POINT_EQUAL(linePoint1.x, linePoint1.y, linePoint2.x, linePoint2.y))
        return Calc2PointsDistance(point, linePoint1);

    LineEquation equa = GetLineEquation(linePoint1, linePoint2);
    //d=|Ax+By+C|/√(A?+B?)
    double ret = fabs(equa.A * point.x + equa.B * point.y + equa.C) / sqrt(equa.A * equa.A + equa.B * equa.B);
    return ret;
}

double MathHelper::CalcLineToXAxisAngle(const Point& point1, const Point& point2)
{
    //360度角 = 2PI弧度  则1弧度 = 180/PI度 
    Point off;
    off.x = point2.x - point1.x;
    off.y = point2.y - point1.y;

    //atan2取值范围为[-PI,PI],即-180到180
    double angle = atan2(off.y, off.x) * 180 / PI;

    //将[-PI,0)的数转为360制
    if (angle < 1e-6)
        angle += 360;
    return angle;
}

int MathHelper::CalcPointToLineSide(const Point& point, const Point& linePoint1, const Point& linePoint2)
{
    /**
    * 定义：平面上的三点P1(x1, y1), P2(x2, y2), P3(x3, y3)的面积量：
    *                 | x1  x2  x3 |
    * S(P1, P2, P3) = | y1  y2  y3 | = (x1 - x3)*(y2 - y3) - (y1 - y3)(x2 - x3)
    *                 | 1   1   1  |
    * 当P1P2P3逆时针时S为正的，当P1P2P3顺时针时S为负的。
    * 
    * 令矢量的起点为A，终点为B，判断的点为C，
    * 如果S（A，B，C）为正数，则C在矢量AB的左侧；
    * 如果S（A，B，C）为负数，则C在矢量AB的右侧；
    * 如果S（A，B，C）为0，则C在直线AB上。
    */
    if (MATH_POINT_EQUAL(linePoint1.x, linePoint1.y, linePoint2.x, linePoint2.y))
        return -1;

    double area = (linePoint1.x - point.x) * (linePoint2.y - point.y) - (linePoint2.x - point.x) * (linePoint1.y - point.y);
    if (area > FLOAT_EQUAL_LIMIT)
        return 2;
    else if (area < FLOAT_EQUAL_LIMIT)
        return 1;
    return 0;
}

Point MathHelper::GetPointFromSegment(const Point& point1, const Point& point2, double disPoint1)
{
    if (disPoint1 <= FLOAT_EQUAL_LIMIT)
        return point1;

    double distrance = Calc2PointsDistance(point1, point2);
    if (distrance <= FLOAT_EQUAL_LIMIT || distrance < disPoint1)
        return point1;

    //如果超过了线段长度，则返回第二个点
    if ((distrance - disPoint1) <= FLOAT_EQUAL_LIMIT || (disPoint1 - distrance) >= FLOAT_EQUAL_LIMIT)
        return point2;

    Point ret;
    ret.x = point1.x - ((disPoint1 * (point1.x - point2.x)) / distrance);
    ret.y = point1.y - ((disPoint1 * (point1.y - point2.y)) / distrance);
    return ret;
}

Point MathHelper::GetVPointFromPointToLine(const Point& point, const Point& linePoint1, const Point& linePoint2)
{
    /**
    * 点A(x,y),线Ax+By+C=0
    * 垂线： y=(B/A)*(x-x0)+y0
    */
    Point ret = { 0, 0 };
    if (MATH_POINT_EQUAL(linePoint1.x, linePoint1.y, linePoint2.x, linePoint2.y))
        return ret;

    //求直线的二元一次方程 Ax+By+C=0
    LineEquation equa = GetLineEquation(linePoint1, linePoint2);

    double x = point.x;
    double y = point.y;
    double A = equa.A;
    double B = equa.B;
    double C = equa.C;

    ret.x = (B * B * x - A * B * y - A * C) / (A * A + B * B);
    ret.y = (-1) * (A * x + C) / B;
    return ret;
}

}
