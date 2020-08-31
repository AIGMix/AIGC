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

namespace AIGC
{
	bool MathHelper::IsBigEndian()
	{
        /**
         * 大端：认为第一个字节是最高位字节
         * 小端：它认为第一个字节是最低位字节
         * x86系列CPU都是小端，PowerPC通常是大端，还有的CPU能通过跳线来设置CPU工作于大端还是小端模式。
         * 判断方法：设置一个int型为1，取第一个字节，如果是1则为小端
         */
		unsigned int iNum = 0xff;
		unsigned char* pTmp = (unsigned char*)&iNum;
		if (*pTmp == 0xff)
			return false;
		return true;
	}
	
	int MathHelper::Calc4ByteAlignSize(int iSize)
	{
        /**
         * 四字节对齐的必要条件是：
         * 1、只能补齐不能减少，比如未对齐之前的长度为1，对齐后只能是4而不是0
         * 2、判断字节对齐的条件是对4求余，如果为0就是对齐
         * 
         * ((in_Size + 3) >> 2) << 2 
         * 加三之后，保证只能被补齐而不会减少。 右移除于4，余数后面的都没了；再左移乘于4，则对齐
         */
		return iSize < 0 ? 0 : ((iSize + 3) >> 2) << 2;  //或者(in_Size + 3 & ~3)
	}

    bool MathHelper::IsSamePoint(const Point &oPoint1, const Point &oPoint2)
    {
        if (MATH_POINT_EQUAL(oPoint1.dX, oPoint1.dY, oPoint2.dX, oPoint2.dY))
            return true;
        return false;
    }

    LineEquation MathHelper::GetLineEquation(const Point &oPoint1, const Point &oPoint2)
    {
        LineEquation ret;
        ret.dA = 0;
        ret.dB = 0;
        ret.dC = 0;
        if (MATH_POINT_EQUAL(oPoint1.dX, oPoint1.dY, oPoint2.dX, oPoint2.dY))
            return ret;

        ret.dA = oPoint2.dY - oPoint1.dY;
        ret.dB = oPoint1.dX - oPoint2.dX;
        ret.dC = oPoint2.dX * oPoint1.dY - oPoint1.dX * oPoint2.dY;
        return ret;
    }

    double MathHelper::Calc2PointsDistance(const Point& oPoint1, const Point& oPoint2)
    {
        if (MATH_POINT_EQUAL(oPoint1.dX, oPoint1.dY, oPoint2.dX, oPoint2.dY))
            return 0.0;

        return sqrt(pow(oPoint2.dX - oPoint1.dX, 2) + pow(oPoint2.dY - oPoint1.dY, 2));
    }

    double MathHelper::CalcPointToLineDistance(const Point& oPoint, const Point& oLinePoint1, const Point& oLinePoint2)
    {
        if (MATH_POINT_EQUAL(oLinePoint1.dX, oLinePoint1.dY, oLinePoint2.dX, oLinePoint2.dY))
            return Calc2PointsDistance(oPoint, oLinePoint1);

        LineEquation equa = GetLineEquation(oLinePoint1, oLinePoint2);
        //d=|Ax+By+C|/√(A?+B?)
        double ret = fabs(equa.dA * oPoint.dX + equa.dB * oPoint.dY + equa.dC) / sqrt(equa.dA * equa.dA + equa.dB * equa.dB);
        return ret;
    }

    double MathHelper::CalcLineToXAxisAngle(const Point& oPoint1, const Point& oPoint2)
    {
        //360度角 = 2PI弧度  则1弧度 = 180/PI度 
        Point off;
        off.dX = oPoint2.dX - oPoint1.dX;
        off.dY = oPoint2.dY - oPoint1.dY;

        //atan2取值范围为[-PI,PI],即-180到180
        double angle = atan2(off.dY, off.dX) * 180 / PI;

        //将[-PI,0)的数转为360制
        if (angle < 1e-6)
            angle += 360;
        return angle;
    }

    int MathHelper::CalcPointToLineSide(const Point& oPoint, const Point& oLinePoint1, const Point& oLinePoint2)
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
        if (MATH_POINT_EQUAL(oLinePoint1.dX, oLinePoint1.dY, oLinePoint2.dX, oLinePoint2.dY))
            return -1;

        double area = (oLinePoint1.dX - oPoint.dX) * (oLinePoint2.dY - oPoint.dY) - (oLinePoint2.dX - oPoint.dX) * (oLinePoint1.dY - oPoint.dY);
        if (area > FLOAT_EQUAL_LIMIT)
            return 2;
        else if (area < FLOAT_EQUAL_LIMIT)
            return 1;
        return 0;
    }

    Point MathHelper::GetPointFromSegment(const Point& oPoint1, const Point& oPoint2, double dDisPoint1)
    {
        if (dDisPoint1 <= FLOAT_EQUAL_LIMIT)
            return oPoint1;

        double distrance = Calc2PointsDistance(oPoint1, oPoint2);
        if (distrance <= FLOAT_EQUAL_LIMIT || distrance < dDisPoint1)
            return oPoint1;

        //如果超过了线段长度，则返回第二个点
        if ((distrance - dDisPoint1) <= FLOAT_EQUAL_LIMIT || (dDisPoint1 - distrance) >= FLOAT_EQUAL_LIMIT)
            return oPoint2;

        Point ret;
        ret.dX = oPoint1.dX - ((dDisPoint1 * (oPoint1.dX - oPoint2.dX)) / distrance);
        ret.dY = oPoint1.dY - ((dDisPoint1 * (oPoint1.dY - oPoint2.dY)) / distrance);
        return ret;
    }

    Point MathHelper::GetVPointFromPointToLine(const Point& oPoint, const Point& oLinePoint1, const Point& oLinePoint2)
    {
        /**
         * 点A(x,y),线Ax+By+C=0
         * 垂线： y=(B/A)*(x-x0)+y0
         */
        Point ret;
        if (MATH_POINT_EQUAL(oLinePoint1.dX, oLinePoint1.dY, oLinePoint2.dX, oLinePoint2.dY))
            return ret;

        //求直线的二元一次方程 Ax+By+C=0
        LineEquation equa = GetLineEquation(oLinePoint1, oLinePoint2);

        double x = oPoint.dX;
        double y = oPoint.dY;
        double A = equa.dA;
        double B = equa.dB;
        double C = equa.dC;

        ret.dX = (B * B * x - A * B * y - A * C) / (A * A + B * B);
        ret.dY = (-1) * (A * x + C) / B;
        return ret;
    }
}
