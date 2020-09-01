#include "MathHelper.h"
#include "StringHelper.h"
#include "PropertyHelper.h"

namespace AIGC
{
    PropertyHelper::PropertyHelper(bool bIgnoreUpLow)
    {
        m_Memory = NULL;
        m_Offset = 0;
        m_IgnoreUpLow = bIgnoreUpLow;
    }

    void PropertyHelper::SetMemory(unsigned char* pMemory)
    {
        m_Memory = pMemory;
    }

    bool PropertyHelper::AddProperty(const std::string &pName, PARA_TYPE eType, int iByteSize)
    {
        Property value = {0, iByteSize};
        std::string name = FormatPropertyName(pName);

        if (m_Properties.size() > 0)
        {
            if (m_Properties.find(name) != m_Properties.end())
                return false;

            value.iByteSize = iByteSize;
            value.iOffset = m_Offset;
            value.eType = eType;
        }

        m_Offset = MathHelper::Calc4ByteAlignSize(value.iOffset + value.iByteSize);
        m_Properties.insert(std::pair<std::string, Property>(name, value));
        return true;
    }

    PropertyHelper::Property PropertyHelper::GetProperty(const std::string& pName)
    {
        PropertyHelper::Property ret = { 0, 0, ePARA_TYPE_NONE };
        std::string name = FormatPropertyName(pName);
        std::map<std::string, Property>::iterator iTor = m_Properties.find(name);
        if (iTor == m_Properties.end())
            return ret;

        Property value = iTor->second;
        return value;
    }

    unsigned char* PropertyHelper::GetValue(const std::string &pName)
    {
        std::string name = FormatPropertyName(pName);
        std::map<std::string, Property>::iterator iTor = m_Properties.find(name);
        if (iTor == m_Properties.end())
            return NULL;

        Property value = iTor->second;
        return (m_Memory + value.iOffset);
    }

    bool PropertyHelper::SetValue(const std::string &pName, unsigned char* oValue)
    {
        return true;
    }

    std::string PropertyHelper::FormatPropertyName(const std::string& pName)
    {
        if(m_IgnoreUpLow)
            return StringHelper::ToLower(pName);
        return pName;
    }

    bool StaticPropertyHelper::HaveClass(const std::string& pClass)
    {
        std::map<std::string, void*>::iterator iTor = m_ClassMap.find(pClass);
        if (iTor == m_ClassMap.end())
            return false;
        return true;
    }

    bool StaticPropertyHelper::EndAddClassProperty(const std::string& pClass)
    {
        if (HaveClass(pClass))
            return true;
        m_ClassMap.insert(std::pair<std::string, void*>(pClass, NULL));
        return false;
    }

    bool StaticPropertyHelper::AddClassProperty(const std::string& pClass, const std::string& pName, PARA_TYPE eType, int iByteSize)
    {
        PropertyHelper* pro;
        std::map<std::string, PropertyHelper>::iterator iTor = m_ClassProperties.find(pClass);
        if (iTor == m_ClassProperties.end())
        {
            pro = &iTor->second;
            pro->AddProperty(pName, eType, iByteSize);
            return true;
        }

        PropertyHelper pnew;
        pnew.AddProperty(pName, eType, iByteSize);
        m_ClassProperties.insert(std::pair<std::string, PropertyHelper>(pClass, pnew));
        return true;
    }


}