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

namespace AIGC
{
	bool MathHelper::IsBigEndian()
	{
        /**
         * ��ˣ���Ϊ��һ���ֽ������λ�ֽ�
         * С�ˣ�����Ϊ��һ���ֽ������λ�ֽ�
         * x86ϵ��CPU����С�ˣ�PowerPCͨ���Ǵ�ˣ����е�CPU��ͨ������������CPU�����ڴ�˻���С��ģʽ��
         * �жϷ���������һ��int��Ϊ1��ȡ��һ���ֽڣ������1��ΪС��
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
         * ���ֽڶ���ı�Ҫ�����ǣ�
         * 1��ֻ�ܲ��벻�ܼ��٣�����δ����֮ǰ�ĳ���Ϊ1�������ֻ����4������0
         * 2���ж��ֽڶ���������Ƕ�4���࣬���Ϊ0���Ƕ���
         * 
         * ((in_Size + 3) >> 2) << 2 
         * ����֮�󣬱�ֻ֤�ܱ������������١� ���Ƴ���4����������Ķ�û�ˣ������Ƴ���4�������
         */
		return iSize < 0 ? 0 : ((iSize + 3) >> 2) << 2;  //����(in_Size + 3 & ~3)
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
        //d=|Ax+By+C|/��(A?+B?)
        double ret = fabs(equa.dA * oPoint.dX + equa.dB * oPoint.dY + equa.dC) / sqrt(equa.dA * equa.dA + equa.dB * equa.dB);
        return ret;
    }

    double MathHelper::CalcLineToXAxisAngle(const Point& oPoint1, const Point& oPoint2)
    {
        //360�Ƚ� = 2PI����  ��1���� = 180/PI�� 
        Point off;
        off.dX = oPoint2.dX - oPoint1.dX;
        off.dY = oPoint2.dY - oPoint1.dY;

        //atan2ȡֵ��ΧΪ[-PI,PI],��-180��180
        double angle = atan2(off.dY, off.dX) * 180 / PI;

        //��[-PI,0)����תΪ360��
        if (angle < 1e-6)
            angle += 360;
        return angle;
    }

    int MathHelper::CalcPointToLineSide(const Point& oPoint, const Point& oLinePoint1, const Point& oLinePoint2)
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

        //����������߶γ��ȣ��򷵻صڶ�����
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
         * ��A(x,y),��Ax+By+C=0
         * ���ߣ� y=(B/A)*(x-x0)+y0
         */
        Point ret;
        if (MATH_POINT_EQUAL(oLinePoint1.dX, oLinePoint1.dY, oLinePoint2.dX, oLinePoint2.dY))
            return ret;

        //��ֱ�ߵĶ�Ԫһ�η��� Ax+By+C=0
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
