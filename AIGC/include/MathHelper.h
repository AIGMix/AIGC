#pragma once
#include <string>

namespace aigc
{
//��
typedef struct PointStruct
{
    double x;
    double y;
}Point;

//�߿�
typedef struct RectStruct
{
    double left;
    double top;
    double right;
    double bottom;
}Rect;

//ֱ�ߵ�һ�㷽��ʽ(Ax+By+C=0)
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
    * @brief �Ƿ�Ϊ���ģʽ
    * @return {bool} �Ƿ�
    */
    static bool IsBigEndian();

    /**
    * @brief ��ȡ���ֽڶ���ĳ���
    * @param {int} size ԭ����
    * @return {int} �����ĳ���
    */
    static int Calc4ByteAlignSize(int size);

    /**
    * @brief �Ƿ�Ϊ��ͬ��
    * @param {Point} point1 ��1
    * @param {Point} point2 ��2
    * @return {bool} �Ƿ�
    */
    static bool IsSamePoint(const Point &point1, const Point &point2);

    /**
    * @brief ��ȡֱ�ߵ�һ�㷽��ʽ
    * @param {Point} point1 ��1
    * @param {Point} point2 ��2
    * @return {LineEquation}
    */
    static LineEquation GetLineEquation(const Point& point1, const Point& point2);

    /**
    * @brief ������������
    * @param {Point} point1 ��1
    * @param {Point} point2 ��2
    * @return {double} ����
    */
    static double Calc2PointsDistance(const Point& point1, const Point& point2);

    /**
    * @brief ����㵽�ߵľ���
    * @param {Point} point ��
    * @param {Point} linePoint1 ���ϵĵ�1
    * @param {Point} linePoint2 ���ϵĵ�2
    * @return {double} ����
    */
    static double CalcPointToLineDistance(const Point& point, const Point& linePoint1, const Point& linePoint2);

    /**
    * @brief ��������X��ļн�
    * @param {Point} point1 ��1
    * @param {Point} point2 ��2
    * @return {double} �Ƕ�
    */
    static double CalcLineToXAxisAngle(const Point &point1, const Point &point2);

    /**
    * @brief �жϵ����ߵ���һ��
    * @param {Point} point ��
    * @param {Point} linePoint1 ���ϵĵ�1
    * @param {Point} linePoint2 ���ϵĵ�2
    * @return {int} 0���� 1��� 2�Ҳ� -1����
    */
    static int CalcPointToLineSide(const Point &point, const Point &linePoint1, const Point &linePoint2);

    /**
    * @brief ��ȡ�߶��ϵĵ�
    * @param {Point} point1 ��1
    * @param {Point} point2 ��2
    * @param {double} disPoint1 �˵㵽��1�ľ���
    * @return {Point} ��
    */
    static Point GetPointFromSegment(const Point& point1, const Point& point2, double disPoint1);

    /**
    * @brief ��ȡ�㵽�ߵĴ���
    * @param {Point} point ��
    * @param {Point} linePoint1 ���ϵĵ�1
    * @param {Point} linePoint2 ���ϵĵ�2
    * @return {Point} ����
    */
    static Point GetVPointFromPointToLine(const Point &point, const Point &linePoint1, const Point &linePoint2);

};
}
