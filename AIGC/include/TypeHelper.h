#pragma once
#include <string>

namespace AIGC
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
         * @param {PARA_TYPE} eType 类型
         * @return {int} 字节数
         */
        static int GetTypeSize(PARA_TYPE eType);

        /**
         * @brief 获取类型名称
         * @param {PARA_TYPE} eType 类型
         * @return {string} 名称
         */
        static std::string GetTypeName(PARA_TYPE eType);
    };
}
