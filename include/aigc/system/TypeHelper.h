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
    static int GetTypeSize(PARA_TYPE type)
    {
        switch (type)
        {
        case PARA_TYPE::INT:
            return sizeof(int);
        case PARA_TYPE::CHAR:
            return sizeof(char);
        case PARA_TYPE::BOOL:
            return sizeof(bool);
        case PARA_TYPE::FLOAT:
            return sizeof(float);
        case PARA_TYPE::DOUBLE:
            return sizeof(double);
        case PARA_TYPE::STRING:
            return 0;
        case PARA_TYPE::PTR:
            return sizeof(void *);
        default:
            return 0;
        }
        return 0;
    }

    /**
     * @brief 获取类型名称
     * @param type 类型
     * @return 名称
     */
    static std::string GetTypeName(PARA_TYPE type)
    {
        switch (type)
        {
        case PARA_TYPE::INT:
            return "int";
        case PARA_TYPE::CHAR:
            return "char";
        case PARA_TYPE::BOOL:
            return "bool";
        case PARA_TYPE::FLOAT:
            return "float";
        case PARA_TYPE::STRING:
            return "string";
        case PARA_TYPE::DOUBLE:
            return "double";
        case PARA_TYPE::PTR:
            return "ptr";
        default:
            return "";
        }
        return "";
    }
};

}
