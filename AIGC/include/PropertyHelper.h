#pragma once
#include <map>
#include <string>
#include "TypeHelper.h"

namespace AIGC
{
    class PropertyHelper
    {
    public:
        typedef struct _Property
        {
            int iOffset;
            int iByteSize;
            PARA_TYPE eType;
        } Property;

        /**
         * @brief 构造函数
         */
        PropertyHelper(bool bIgnoreUpLow = true);

        /**
         * @brief 设置缓存
         * @param {unsigned char *} pMemory 缓存
         */
        void SetMemory(unsigned char * pMemory);

        /**
         * @brief 添加成员
         * @param {string} pName 名称
         * @param {iByteSize} 所占字节数
         * @return {bool} 成败
         */
        bool AddProperty(const std::string &pName, PARA_TYPE eType, int iByteSize);

        /**
         * @brief 获取成员
         * @param {string} pName 名称
         * @return {Property} 
         */
        Property GetProperty(const std::string& pName);

        /**
         * @brief 获取成员值
         * @param {string} pName 名称
         * @return {PARA_TYPE} 值
         */
        unsigned char* GetValue(const std::string& pName);

        /**
         * @brief 设置成员值
         * @param {string} pName 名称
         * @param {unsigned char*} oValue 值
         * @return {PARA_TYPE} 值
         */
        bool SetValue(const std::string &pName, unsigned char* oValue);


    private:
        /**
         * @brief 格式化成员名称
         * @param {string} pName 名称
         * @return {string}
         */
        std::string FormatPropertyName(const std::string& pName);

        int m_Offset;
        bool m_IgnoreUpLow;
        unsigned char* m_Memory;
        std::map<std::string, Property> m_Properties;
    };


    class StaticPropertyHelper
    {
    public:
        static bool HaveClass(const std::string& pClass);
        static bool EndAddClassProperty(const std::string& pClass);
        static bool AddClassProperty(const std::string& pClass, const std::string& pName, PARA_TYPE eType, int iByteSize);

    private:
        static std::map<std::string, PropertyHelper> m_ClassProperties;
        static std::map<std::string, void*> m_ClassMap;
    };
}