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

namespace AIGC
{
    static rapidjson::Value GetJsonValue(std::string sStr, std::vector<std::string> sNames)
    {
        rapidjson::Document doc;
        doc.Parse(sStr.c_str());

        rapidjson::Value value;
        for (std::vector<std::string>::iterator it = sNames.begin(); it != sNames.end(); ++it)
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

    std::string JsonHelper::GetValue(std::string sStr, std::vector<std::string> sNames, std::string sDefault)
    {
        rapidjson::Value value = GetJsonValue(sStr, sNames);
        if (value.IsNull() || !value.IsString())
            return sDefault;
        return value.GetString();
    }

    int JsonHelper::GetValueInt(std::string sStr, std::vector<std::string> sNames, int iDefault)
    {
        rapidjson::Value value = GetJsonValue(sStr, sNames);
        if (value.IsNull() || !value.IsInt())
            return iDefault;
        return value.GetInt();
    }

    double JsonHelper::GetValueDouble(std::string sStr, std::vector<std::string> sNames, double dDefault)
    {
        rapidjson::Value value = GetJsonValue(sStr, sNames);
        if (value.IsNull() || !value.IsDouble())
            return dDefault;
        return value.GetDouble();
    }

    bool JsonHelper::GetValueBool(std::string sStr, std::vector<std::string> sNames, bool bDefault)
    {
        rapidjson::Value value = GetJsonValue(sStr, sNames);
        if (value.IsNull() || !value.IsBool())
            return bDefault;
        return value.GetBool();
    }
}