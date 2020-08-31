#pragma once
#include <string>

namespace AIGC
{
    //��
    typedef struct _Point
    {
        double dX;
        double dY;
    }Point;

    //�߿�
    typedef struct _Rect
    {
        double dLeft;
        double dTop;
        double dRight;
        double dBottom;
    }Rect;

    //ֱ�ߵ�һ�㷽��ʽ(Ax+By+C=0)
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
         * @brief �Ƿ�Ϊ���ģʽ
         * @return {bool} �Ƿ�
         */
        static bool IsBigEndian();

        /**
         * @brief ��ȡ���ֽڶ���ĳ���
         * @param {int} iSize ԭ����
         * @return {int} �����ĳ���
         */
        static int Calc4ByteAlignSize(int iSize);



        /**
         * @brief ������������
         * @param {Point} oPoint1 ��1
         * @param {Point} oPoint2 ��2
         * @return {double} ����
         */
        static double Calc2PointsDistance(const Point& oPoint1, const Point& oPoint2);

        /**
         * @brief ����㵽�ߵľ���
         * @param {Point} oPoint ��
         * @param {Point} oLinePoint1 ���ϵĵ�1
         * @param {Point} oLinePoint2 ���ϵĵ�2
         * @return {double} ����
         */
        static double CalcPointToLineDistance(const Point& oPoint, const Point& oLinePoint1, const Point& oLinePoint2);



        /**
         * @brief ��ȡ�߶��ϵĵ�
         * @param {Point} oPoint1 ��1
         * @param {Point} oPoint2 ��2
         * @param {double} dDisPoint1 �˵㵽��1�ľ���
         * @return {Point} ��
         */
        static Point GetPointFromSegment(const Point& oPoint1, const Point& oPoint2, double dDisPoint1);
    };
}
