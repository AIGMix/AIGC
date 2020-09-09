#pragma once
#include <string>

namespace aigc
{
    
enum class PARA_TYPE
{
    NONE = 0,
    INT,
    CHAR,
    BOOL,
    FLOAT,
    DOUBLE,
    STRING,
    PTR,
};

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
