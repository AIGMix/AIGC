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

}
