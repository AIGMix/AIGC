#pragma once
#include <string>

namespace aigc
{
    
typedef enum _PARA_TYPE
{
    ePARA_TYPE_NONE = 0,
    ePARA_TYPE_INT,
    ePARA_TYPE_CHAR,
    ePARA_TYPE_BOOL,
    ePARA_TYPE_FLOAT,
    ePARA_TYPE_STRING,
    ePARA_TYPE_DOUBLE,
    ePARA_TYPE_PTR,
}PARA_TYPE;

class TypeHelper
{
public:
    /**
     * @brief 获取类型字节大小
     * @param type 类型
     * @return 字节数
     */
    static int GetTypeSize(PARA_TYPE type);

    /**
     * @brief 获取类型名称
     * @param type 类型
     * @return 名称
     */
    static std::string GetTypeName(PARA_TYPE type);
};

}
