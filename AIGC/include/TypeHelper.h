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
         * @brief ��ȡ�����ֽڴ�С
         * @param {PARA_TYPE} eType ����
         * @return {int} �ֽ���
         */
        static int GetTypeSize(PARA_TYPE eType);

        /**
         * @brief ��ȡ��������
         * @param {PARA_TYPE} eType ����
         * @return {string} ����
         */
        static std::string GetTypeName(PARA_TYPE eType);
    };
}
