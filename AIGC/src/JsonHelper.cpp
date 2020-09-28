/**********
* File    :   JsonHelper.cpp
* Time    :   2020/08/26
* Author  :   Yaronzz
* Version :   1.0
* Contact :   yaronhuang@foxmail.com
* Desc    :   
**********/
#include "JsonHelper.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <stdarg.h>


namespace aigc
{
    JsonHelper::JsonHelper(std::string jsonStr)
    {
        LoadJsonString(jsonStr);
    }

    JsonHelper::~JsonHelper()
    {
    }

    bool JsonHelper::LoadJsonString(std::string jsonStr)
    {
        if (jsonStr != "")
            m_doc.Parse(jsonStr.c_str());
        return !m_doc.IsNull();
    }

    bool JsonHelper::GetValue(const strings<const std::string>& names, rapidjson::Value& value)
    {
        if (m_doc.IsNull())
            return false;

        rapidjson::Value newValue;
        for (const std::string* key = names.begin(); key != names.end(); ++key)
        {
            const char *find = (*key).c_str();
            if (newValue.IsNull())
            {
                if (!m_doc.HasMember(find))
                    return false;
                newValue = m_doc[find];
            }
            else
            {
                if (newValue.IsNull() || !newValue.IsObject() || !newValue.HasMember(find))
                    return false;
                newValue = newValue[find];
            }
        }

        if (newValue.IsNull())
            return false;
        value = newValue;
        return true;
    }

    std::string JsonHelper::GetString(const strings<const std::string> names, std::string defaultV)
    {
        rapidjson::Value value;
        if (!GetValue(names, value) || !value.IsString())
            return defaultV;
        return value.GetString();
    }

    int JsonHelper::GetInt(strings<const std::string> names, int defaultV)
    {
        rapidjson::Value value;
        if (!GetValue(names, value) || !value.IsInt())
            return defaultV;
        return value.GetInt();
    }

    double JsonHelper::GetDouble(strings<const std::string> names, double defaultV)
    {
        rapidjson::Value value;
        if (!GetValue(names, value) || !value.IsDouble())
            return defaultV;
        return value.GetDouble();
    }

    bool JsonHelper::GetBool(strings<const std::string> names, bool defaultV)
    {
        rapidjson::Value value;
        if (!GetValue(names, value) || !value.IsBool())
            return defaultV;
        return value.GetBool();
    }

    void JsonHelper::InitMembers(std::string names)
    {
        if (m_members.size() > 0)
            return;

        m_members = StringHelper::Split(names, ',');
        for (int i = 0; i < m_members.size(); i++)
        {
            m_members[i] = StringHelper::Trim(m_members[i]);
        }
    }

    bool JsonHelper::SetValue(int &person, rapidjson::Value &value)
    {
        if (value.IsNull() || !value.IsInt())
            return false;
        person = value.GetInt();
        return true;
    }
    bool JsonHelper::SetValue(bool &person, rapidjson::Value &value)
    {
        if (value.IsNull() || !value.IsBool())
            return false;
        person = value.GetBool();
        return true;
    }
    bool JsonHelper::SetValue(double &person, rapidjson::Value &value)
    {
        if (value.IsNull() || !value.IsDouble())
            return false;
        person = value.GetDouble();
        return true;
    }
    bool JsonHelper::SetValue(std::string &person, rapidjson::Value &value)
    {
        if (value.IsNull() || !value.IsString())
            return false;
        person = value.GetString();
        return true;
    }
}

//using namespace aigc;
//
//int main()
//{
//    std::string buf(R"({"hello":{"1":"test1","2":"test2"},"world":"world"})");
//    JsonHelper test(buf);
//    std::string value = test.GetString({ "hello","1" });
//    return 0;
//}

// class Student : public JsonHelper
// {
// public:
//     string Name;
//     int Age;

//     JSON_HELPER_LOAD_MEMBERS(Name, Age)
// };

// int main()
// {

//     string tjson = "{	\"Name\" : \"Boo\",	\"Age\" : 28}";
//     Student test;
//     test.Name = "my";
//     test.Age = 12;
//     test.JsonToClass(tjson);
//     return 0;
// }