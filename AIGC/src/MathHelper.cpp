#include <math.h>
#include "MathHelper.h"

//Բ����
#define PI									 3.14159265359
//�������ı߽�
#define FLOAT_EQUAL_LIMIT					 0.0000001f	

//�����ֵ
#define MATH_ABS(value)			         ((value) > 0 ? (value) : -(value))
//���ֵ
#define MATH_MAX(A, B)					 (A > B ? A : B)
//��Сֵ
#define MATH_MIN(A, B)					 (A > B ? B : A)

//�������Ƿ���ͬ
#define MATH_FLOAT_EQUAL(A, B)			 (MATH_ABS(A - B) < FLOAT_EQUAL_LIMIT)
//���Ƿ���ͬ
#define MATH_POINT_EQUAL(AX, AY, BX, BY) (MATH_FLOAT_EQUAL(AX, BX) && MATH_FLOAT_EQUAL(AY, BY))

namespace aigc
{
bool MathHelper::IsBigEndian()
{
    /**
    * ��ˣ���Ϊ��һ���ֽ������λ�ֽ�
    * С�ˣ�����Ϊ��һ���ֽ������λ�ֽ�
    * x86ϵ��CPU����С�ˣ�PowerPCͨ���Ǵ�ˣ����е�CPU��ͨ������������CPU�����ڴ�˻���С��ģʽ��
    * �жϷ���������һ��int��Ϊ1��ȡ��һ���ֽڣ������1��ΪС��
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
    * ���ֽڶ���ı�Ҫ�����ǣ�
    * 1��ֻ�ܲ��벻�ܼ��٣�����δ����֮ǰ�ĳ���Ϊ1�������ֻ����4������0
    * 2���ж��ֽڶ���������Ƕ�4���࣬���Ϊ0���Ƕ���
    * 
    * ((in_Size + 3) >> 2) << 2 
    * ����֮�󣬱�ֻ֤�ܱ������������١� ���Ƴ���4����������Ķ�û�ˣ������Ƴ���4�������
    */
    return size < 0 ? 0 : ((size + 3) >> 2) << 2;  //����(in_Size + 3 & ~3)
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
    //d=|Ax+By+C|/��(A?+B?)
    double ret = fabs(equa.A * point.x + equa.B * point.y + equa.C) / sqrt(equa.A * equa.A + equa.B * equa.B);
    return ret;
}

double MathHelper::CalcLineToXAxisAngle(const Point& point1, const Point& point2)
{
    //360�Ƚ� = 2PI����  ��1���� = 180/PI�� 
    Point off;
    off.x = point2.x - point1.x;
    off.y = point2.y - point1.y;

    //atan2ȡֵ��ΧΪ[-PI,PI],��-180��180
    double angle = atan2(off.y, off.x) * 180 / PI;

    //��[-PI,0)����תΪ360��
    if (angle < 1e-6)
        angle += 360;
    return angle;
}

int MathHelper::CalcPointToLineSide(const Point& point, const Point& linePoint1, const Point& linePoint2)
{
    /**
    * ���壺ƽ���ϵ�����P1(x1, y1), P2(x2, y2), P3(x3, y3)���������
    *                 | x1  x2  x3 |
    * S(P1, P2, P3) = | y1  y2  y3 | = (x1 - x3)*(y2 - y3) - (y1 - y3)(x2 - x3)
    *                 | 1   1   1  |
    * ��P1P2P3��ʱ��ʱSΪ���ģ���P1P2P3˳ʱ��ʱSΪ���ġ�
    * 
    * ��ʸ�������ΪA���յ�ΪB���жϵĵ�ΪC��
    * ���S��A��B��C��Ϊ��������C��ʸ��AB����ࣻ
    * ���S��A��B��C��Ϊ��������C��ʸ��AB���Ҳࣻ
    * ���S��A��B��C��Ϊ0����C��ֱ��AB�ϡ�
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

    //����������߶γ��ȣ��򷵻صڶ�����
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
    * ��A(x,y),��Ax+By+C=0
    * ���ߣ� y=(B/A)*(x-x0)+y0
    */
    Point ret = { 0, 0 };
    if (MATH_POINT_EQUAL(linePoint1.x, linePoint1.y, linePoint2.x, linePoint2.y))
        return ret;

    //��ֱ�ߵĶ�Ԫһ�η��� Ax+By+C=0
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
