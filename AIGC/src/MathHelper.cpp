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

}
