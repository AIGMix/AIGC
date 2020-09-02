/**********
* File    :   JsonHelper.cpp
* Time    :   2020/08/26
* Author  :   Yaronzz
* Version :   1.0
* Contact :   yaronhuang@foxmail.com
* Desc    :   
**********/

#include "JsonHelper.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace aigc
{
static rapidjson::Value GetJsonValue(std::string str, std::vector<std::string> names)
{
    rapidjson::Document doc;
    doc.Parse(str.c_str());

    rapidjson::Value value;
    for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it)
    {
        const char *find = (*it).c_str();
        if (value.IsNull())
        {
            if (!doc.HasMember(find))
                return rapidjson::Value();
            value = doc[find];
        }
        else
        {
            if (value.IsNull() || !value.IsObject() || !value.HasMember(find))
                return rapidjson::Value();
            value = value[find];
        }
    }
    return value;
}

std::string JsonHelper::GetValue(std::string str, std::vector<std::string> names, std::string defaultValue)
{
    rapidjson::Value value = GetJsonValue(str, names);
    if (value.IsNull() || !value.IsString())
        return defaultValue;
    return value.GetString();
}

int JsonHelper::GetValueInt(std::string str, std::vector<std::string> names, int defaultValue)
{
    rapidjson::Value value = GetJsonValue(str, names);
    if (value.IsNull() || !value.IsInt())
        return defaultValue;
    return value.GetInt();
}

double JsonHelper::GetValueDouble(std::string str, std::vector<std::string> names, double defaultValue)
{
    rapidjson::Value value = GetJsonValue(str, names);
    if (value.IsNull() || !value.IsDouble())
        return defaultValue;
    return value.GetDouble();
}

bool JsonHelper::GetValueBool(std::string str, std::vector<std::string> names, bool defaultValue)
{
    rapidjson::Value value = GetJsonValue(str, names);
    if (value.IsNull() || !value.IsBool())
        return defaultValue;
    return value.GetBool();
}
}