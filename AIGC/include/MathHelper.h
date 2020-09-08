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
    * @return �Ƿ�
    */
    static bool IsBigEndian();

    /**
    * @brief ��ȡ���ֽڶ���ĳ���
    * @param size ԭ����
    * @return �����ĳ���
    */
    static int Calc4ByteAlignSize(int size);

    /**
    * @brief �Ƿ�Ϊ��ͬ��
    * @param point1 ��1
    * @param point2 ��2
    * @return �Ƿ�
    */
    static bool IsSamePoint(const Point &point1, const Point &point2);

    /**
    * @brief ��ȡֱ�ߵ�һ�㷽��ʽ
    * @param point1 ��1
    * @param point2 ��2
    */
    static LineEquation GetLineEquation(const Point& point1, const Point& point2);

    /**
    * @brief ������������
    * @param point1 ��1
    * @param point2 ��2
    * @return ����
    */
    static double Calc2PointsDistance(const Point& point1, const Point& point2);

    /**
    * @brief ����㵽�ߵľ���
    * @param point ��
    * @param linePoint1 ���ϵĵ�1
    * @param linePoint2 ���ϵĵ�2
    * @return ����
    */
    static double CalcPointToLineDistance(const Point& point, const Point& linePoint1, const Point& linePoint2);

    /**
    * @brief ��������X��ļн�
    * @param point1 ��1
    * @param point2 ��2
    * @return �Ƕ�
    */
    static double CalcLineToXAxisAngle(const Point &point1, const Point &point2);

    /**
    * @brief �жϵ����ߵ���һ��
    * @param point ��
    * @param linePoint1 ���ϵĵ�1
    * @param linePoint2 ���ϵĵ�2
    * @return 0���� 1��� 2�Ҳ� -1����
    */
    static int CalcPointToLineSide(const Point &point, const Point &linePoint1, const Point &linePoint2);

    /**
    * @brief ��ȡ�߶��ϵĵ�
    * @param point1 ��1
    * @param point2 ��2
    * @param disPoint1 �˵㵽��1�ľ���
    * @return ��
    */
    static Point GetPointFromSegment(const Point& point1, const Point& point2, double disPoint1);

    /**
    * @brief ��ȡ�㵽�ߵĴ���
    * @param point ��
    * @param linePoint1 ���ϵĵ�1
    * @param linePoint2 ���ϵĵ�2
    * @return ����
    */
    static Point GetVPointFromPointToLine(const Point &point, const Point &linePoint1, const Point &linePoint2);

};
}
