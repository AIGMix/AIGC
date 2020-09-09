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
     * @brief ��ȡ�����ֽڴ�С
     * @param type ����
     * @return �ֽ���
     */
    static int GetTypeSize(PARA_TYPE type);

    /**
     * @brief ��ȡ��������
     * @param type ����
     * @return ����
     */
    static std::string GetTypeName(PARA_TYPE type);
};

}
