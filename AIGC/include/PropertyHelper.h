#pragma once
#include <map>
#include <string>
#include "TypeHelper.h"

namespace aigc
{
class PropertyHelper
{
public:
    typedef struct PropertyStruct
    {
        int offset;
        int byteSize;
        PARA_TYPE type;
    } Property;

    /**
     * @brief 构造函数
     */
    PropertyHelper(bool ignoreUpLow = true);

    /**
     * @brief 设置缓存
     * @param {unsigned char *} memory 缓存
     */
    void SetMemory(unsigned char * memory);

    /**
     * @brief 添加成员
     * @param {string} name 名称
     * @param {int} byteSize 所占字节数
     * @return {bool} 成败
     */
    bool AddProperty(const std::string &name, PARA_TYPE type, int byteSize);

    /**
     * @brief 获取成员
     * @param {string} name 名称
     * @return {Property} 
     */
    Property GetProperty(const std::string& name);

    /**
     * @brief 获取成员值
     * @param {string} name 名称
     * @return {PARA_TYPE} 值
     */
    unsigned char* GetValue(const std::string& name);

    /**
     * @brief 设置成员值
     * @param {string} name 名称
     * @param {unsigned char*} value 值
     * @return {PARA_TYPE} 值
     */
    bool SetValue(const std::string &name, unsigned char* value);


private:
    /**
     * @brief 格式化成员名称
     * @param {string} name 名称
     * @return {string}
     */
    std::string FormatPropertyName(const std::string& name);

    int m_offset;
    bool m_ignoreUpLow;
    unsigned char* m_memory;
    std::map<std::string, Property> m_properties;
};

}