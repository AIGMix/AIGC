#include "StringHelper.h"
#include "TypeHelper.h"

namespace aigc
{

int TypeHelper::GetTypeSize(PARA_TYPE type)
{
    switch (type)
    {
    case PARA_TYPE::INT:
        return sizeof(int);
        break;
    case PARA_TYPE::CHAR:
        return sizeof(char);
        break;
    case PARA_TYPE::BOOL:
        return sizeof(bool);
        break;
    case PARA_TYPE::FLOAT:
        return sizeof(float);
        break;
    case PARA_TYPE::DOUBLE:
        return sizeof(double);
        break;
    case PARA_TYPE::STRING:
        return 0;
        break;
    case PARA_TYPE::PTR:
        return sizeof(void *);
        break;
    default:
        return 0;
    }
    return 0;
}

std::string TypeHelper::GetTypeName(PARA_TYPE type)
{
    switch (type)
    {
    case PARA_TYPE::INT:
        return "int";
        break;
    case PARA_TYPE::CHAR:
        return "char";
        break;
    case PARA_TYPE::BOOL:
        return "bool";
        break;
    case PARA_TYPE::FLOAT:
        return "float";
        break;
    case PARA_TYPE::STRING:
        return "string";
        break;
    case PARA_TYPE::DOUBLE:
        return "double";
        break;
    case PARA_TYPE::PTR:
        return "ptr";
        break;
    default: return "";
    }
    return "";
}

}