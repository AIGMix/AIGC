#include "MathHelper.h"
#include "StringHelper.h"
#include "PropertyHelper.h"

namespace aigc
{
    PropertyHelper::PropertyHelper(bool ignoreUpLow)
    {
        m_memory = NULL;
        m_offset = 0;
        m_ignoreUpLow = ignoreUpLow;
    }

    void PropertyHelper::SetMemory(unsigned char* memory)
    {
        m_memory = memory;
    }

    bool PropertyHelper::AddProperty(const std::string &name, PARA_TYPE type, int byteSize)
    {
        Property value = {0, byteSize};
        std::string newName = FormatPropertyName(name);

        if (m_properties.size() > 0)
        {
            if (m_properties.find(newName) != m_properties.end())
                return false;

            value.byteSize = byteSize;
            value.offset = m_offset;
            value.type = type;
        }

        m_offset = MathHelper::Calc4ByteAlignSize(value.offset + value.byteSize);
        m_properties.insert(std::pair<std::string, Property>(newName, value));
        return true;
    }

    PropertyHelper::Property PropertyHelper::GetProperty(const std::string& name)
    {
        PropertyHelper::Property ret = { 0, 0, PARA_TYPE::NONE };
        std::string newName = FormatPropertyName(name);
        std::map<std::string, Property>::iterator iTor = m_properties.find(newName);
        if (iTor == m_properties.end())
            return ret;

        Property value = iTor->second;
        return value;
    }

    unsigned char* PropertyHelper::GetValue(const std::string &name)
    {
        std::string newName = FormatPropertyName(name);
        std::map<std::string, Property>::iterator iTor = m_properties.find(newName);
        if (iTor == m_properties.end())
            return NULL;

        Property value = iTor->second;
        return (m_memory + value.offset);
    }

    bool PropertyHelper::SetValue(const std::string &name, unsigned char* value)
    {
        return true;
    }

    std::string PropertyHelper::FormatPropertyName(const std::string& name)
    {
        if(m_ignoreUpLow)
            return StringHelper::ToLower(name);
        return name;
    }


}