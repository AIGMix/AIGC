#include "JsonHelper.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace AIGC
{
	std::string JsonHelper::GetValue(std::string sStr, std::vector<std::string> sNames)
	{
		rapidjson::Document doc;
		doc.Parse(sStr.c_str());
		
		rapidjson::Value value;
		for (std::vector<std::string>::iterator it = sNames.begin(); it != sNames.end(); ++it)
		{ 
			const char* find = (*it).c_str();
			if (value.IsNull())
			{
				if (!doc.HasMember(find))
					return "";
				value = doc[find];
			}
			else
			{
				if (!value.HasMember(find))
					return "";
				value = value[find];
			}
		}
		
		/*if (value.IsBool())
		{
			value.GetBool();
		}*/
	}

}