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
         * @brief �Ƿ�Ϊ��ͬ��
         * @param {Point} oPoint1 ��1
         * @param {Point} oPoint2 ��2
         * @return {bool} �Ƿ�
         */
        static bool IsSamePoint(const Point &oPoint1, const Point &oPoint2);

        /**
         * @brief ��ȡֱ�ߵ�һ�㷽��ʽ
         * @param {Point} oPoint1 ��1
         * @param {Point} oPoint2 ��2
         * @return {LineEquation}
         */
        static LineEquation GetLineEquation(const Point& oPoint1, const Point& oPoint2);

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
         * @brief ��������X��ļн�
         * @param {Point} oPoint1 ��1
         * @param {Point} oPoint2 ��2
         * @return {double} �Ƕ�
         */
        static double CalcLineToXAxisAngle(const Point &oPoint1, const Point &oPoint2);

        /**
         * @brief �жϵ����ߵ���һ��
         * @param {Point} oPoint ��
         * @param {Point} oLinePoint1 ���ϵĵ�1
         * @param {Point} oLinePoint2 ���ϵĵ�2
         * @return {int} 0���� 1��� 2�Ҳ� -1����
         */
        static int CalcPointToLineSide(const Point &oPoint, const Point &oLinePoint1, const Point &oLinePoint2);

        /**
         * @brief ��ȡ�߶��ϵĵ�
         * @param {Point} oPoint1 ��1
         * @param {Point} oPoint2 ��2
         * @param {double} dDisPoint1 �˵㵽��1�ľ���
         * @return {Point} ��
         */
        static Point GetPointFromSegment(const Point& oPoint1, const Point& oPoint2, double dDisPoint1);

        /**
         * @brief ��ȡ�㵽�ߵĴ���
         * @param {Point} oPoint ��
         * @param {Point} oLinePoint1 ���ϵĵ�1
         * @param {Point} oLinePoint2 ���ϵĵ�2
         * @return {Point} ����
         */
        static Point GetVPointFromPointToLine(const Point &oPoint, const Point &oLinePoint1, const Point &oLinePoint2);


    };
}
