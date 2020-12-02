#pragma once
#include <string>

namespace aigc
{

//Բ����
#define AIGC_MATH_PI 3.14159265359

//�������ı߽�
#define AIGC_MATH_FLOAT_EQUAL_LIMIT 0.0000001f

//�����ֵ | ���ֵ | ��Сֵ
#define AIGC_MATH_ABS(value) ((value) > 0 ? (value) : -(value))
#define AIGC_MATH_MAX(A, B) (A > B ? A : B)
#define AIGC_MATH_MIN(A, B) (A > B ? B : A)

//�������Ƿ���ͬ | ���Ƿ���ͬ
#define AIGC_MATH_FLOAT_EQUAL(A, B) (AIGC_MATH_ABS(A - B) < AIGC_MATH_FLOAT_EQUAL_LIMIT)
#define AIGC_MATH_POINT_EQUAL(AX, AY, BX, BY) (AIGC_MATH_FLOAT_EQUAL(AX, BX) && AIGC_MATH_FLOAT_EQUAL(AY, BY))

    //��
    typedef struct PointStruct
    {
        double x;
        double y;
    } Point;

    //�߿�
    typedef struct RectStruct
    {
        double left;
        double top;
        double right;
        double bottom;
    } Rect;

    //ֱ�ߵ�һ�㷽��ʽ(Ax+By+C=0)
    typedef struct LineEquationStruct
    {
        double A;
        double B;
        double C;
    } LineEquation;

    class MathHelper
    {
    public:
        /**
         * @brief �Ƿ�Ϊ���ģʽ
         */
        static bool IsBigEndian()
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

        /**
         * @brief ��ȡ���ֽڶ���ĳ���
         * @param size ԭ����
         * @return �����ĳ���
         */
        static int Calc4ByteAlignSize(int size)
        {
            /**
             * ���ֽڶ���ı�Ҫ�����ǣ�
             * 1��ֻ�ܲ��벻�ܼ��٣�����δ����֮ǰ�ĳ���Ϊ1�������ֻ����4������0
             * 2���ж��ֽڶ���������Ƕ�4���࣬���Ϊ0���Ƕ���
             * 
             * ((in_Size + 3) >> 2) << 2 
             * ����֮�󣬱�ֻ֤�ܱ������������١� ���Ƴ���4����������Ķ�û�ˣ������Ƴ���4�������
             */
            return size < 0 ? 0 : ((size + 3) >> 2) << 2; //����(in_Size + 3 & ~3)
        }

        /**
         * @brief �Ƿ�Ϊ��ͬ��
         * @param point1 ��1
         * @param point2 ��2
         */
        static bool IsSamePoint(const Point &point1, const Point &point2)
        {
            if (AIGC_MATH_POINT_EQUAL(point1.x, point1.y, point2.x, point2.y))
                return true;
            return false;
        }

        /**
         * @brief ��ȡֱ�ߵ�һ�㷽��ʽ
         * @param point1 ��1
         * @param point2 ��2
         */
        static LineEquation GetLineEquation(const Point &point1, const Point &point2)
        {
            LineEquation ret = { 0, 0, 0, };
            if (AIGC_MATH_POINT_EQUAL(point1.x, point1.y, point2.x, point2.y))
                return ret;

            ret.A = point2.y - point1.y;
            ret.B = point1.x - point2.x;
            ret.C = point2.x * point1.y - point1.x * point2.y;
            return ret;
        }

        /**
         * @brief ������������
         * @param point1 ��1
         * @param point2 ��2
         * @return ����
         */
        static double Calc2PointsDistance(const Point &point1, const Point &point2)
        {
            if (AIGC_MATH_POINT_EQUAL(point1.x, point1.y, point2.x, point2.y))
                return 0.0;

            return sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));
        }

        /**
         * @brief ����㵽�ߵľ���
         * @param point ��
         * @param linePoint1 ���ϵĵ�1
         * @param linePoint2 ���ϵĵ�2
         * @return ����
         */
        static double CalcPointToLineDistance(const Point &point, const Point &linePoint1, const Point &linePoint2)
        {
            if (AIGC_MATH_POINT_EQUAL(linePoint1.x, linePoint1.y, linePoint2.x, linePoint2.y))
                return Calc2PointsDistance(point, linePoint1);

            LineEquation equa = GetLineEquation(linePoint1, linePoint2);
            //d=|Ax+By+C|/��(A?+B?)
            double ret = fabs(equa.A * point.x + equa.B * point.y + equa.C) / sqrt(equa.A * equa.A + equa.B * equa.B);
            return ret;
        }

        /**
         * @brief ��������X��ļн�
         * @param point1 ��1
         * @param point2 ��2
         * @return �Ƕ�
         */
        static double CalcLineToXAxisAngle(const Point &point1, const Point &point2)
        {
            //360�Ƚ� = 2PI����  ��1���� = 180/PI��
            Point off;
            off.x = point2.x - point1.x;
            off.y = point2.y - point1.y;

            //atan2ȡֵ��ΧΪ[-PI,PI],��-180��180
            double angle = atan2(off.y, off.x) * 180 / AIGC_MATH_PI;

            //��[-PI,0)����תΪ360��
            if (angle < 1e-6)
                angle += 360;
            return angle;
        }

        /**
         * @brief �жϵ����ߵ���һ��
         * @param point ��
         * @param linePoint1 ���ϵĵ�1
         * @param linePoint2 ���ϵĵ�2
         * @return 0���� 1��� 2�Ҳ� -1����
         */
        static int CalcPointToLineSide(const Point &point, const Point &linePoint1, const Point &linePoint2)
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
            if (AIGC_MATH_POINT_EQUAL(linePoint1.x, linePoint1.y, linePoint2.x, linePoint2.y))
                return -1;

            double area = (linePoint1.x - point.x) * (linePoint2.y - point.y) - (linePoint2.x - point.x) * (linePoint1.y - point.y);
            if (area > AIGC_MATH_FLOAT_EQUAL_LIMIT)
                return 2;
            else if (area < AIGC_MATH_FLOAT_EQUAL_LIMIT)
                return 1;
            return 0;
        }

        /**
         * @brief ��ȡ�߶��ϵĵ�
         * @param point1 ��1
         * @param point2 ��2
         * @param disPoint1 �˵㵽��1�ľ���
         * @return ��
         */
        static Point GetPointFromSegment(const Point &point1, const Point &point2, double disPoint1)
        {
            if (disPoint1 <= AIGC_MATH_FLOAT_EQUAL_LIMIT)
                return point1;

            double distrance = Calc2PointsDistance(point1, point2);
            if (distrance <= AIGC_MATH_FLOAT_EQUAL_LIMIT || distrance < disPoint1)
                return point1;

            //����������߶γ��ȣ��򷵻صڶ�����
            if ((distrance - disPoint1) <= AIGC_MATH_FLOAT_EQUAL_LIMIT || (disPoint1 - distrance) >= AIGC_MATH_FLOAT_EQUAL_LIMIT)
                return point2;

            Point ret;
            ret.x = point1.x - ((disPoint1 * (point1.x - point2.x)) / distrance);
            ret.y = point1.y - ((disPoint1 * (point1.y - point2.y)) / distrance);
            return ret;
        }

        /**
         * @brief ��ȡ�㵽�ߵĴ���
         * @param point ��
         * @param linePoint1 ���ϵĵ�1
         * @param linePoint2 ���ϵĵ�2
         * @return ����
         */
        static Point GetVPointFromPointToLine(const Point &point, const Point &linePoint1, const Point &linePoint2)
        {
            /**
             * ��A(x,y),��Ax+By+C=0
             * ���ߣ� y=(B/A)*(x-x0)+y0
             */
            Point ret = {0, 0};
            if (AIGC_MATH_POINT_EQUAL(linePoint1.x, linePoint1.y, linePoint2.x, linePoint2.y))
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
    };
} // namespace aigc
