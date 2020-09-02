#include "StringHelper.h"
#include "TypeHelper.h"

namespace aigc
{

int TypeHelper::GetTypeSize(PARA_TYPE type)
{
    switch (type)
    {
    case ePARA_TYPE_INT:return sizeof(int); break;
    case ePARA_TYPE_CHAR:return sizeof(char); break;
    case ePARA_TYPE_BOOL:return sizeof(bool); break;
    case ePARA_TYPE_FLOAT:return sizeof(float); break;
    case ePARA_TYPE_STRING:return 0; break;
    case ePARA_TYPE_DOUBLE:return sizeof(double); break;
    case ePARA_TYPE_PTR:return sizeof(void*); break;
    default: return 0;
    }
    return 0;
}

std::string TypeHelper::GetTypeName(PARA_TYPE type)
{
    switch (type)
    {
    case ePARA_TYPE_INT:return "int"; break;
    case ePARA_TYPE_CHAR:return "char"; break;
    case ePARA_TYPE_BOOL:return "bool"; break;
    case ePARA_TYPE_FLOAT:return "float"; break;
    case ePARA_TYPE_STRING:return "string"; break;
    case ePARA_TYPE_DOUBLE:return "double"; break;
    case ePARA_TYPE_PTR:return "ptr"; break;
    default: return "";
    }
    return "";
}

}