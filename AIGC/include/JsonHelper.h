#pragma once

#include <vector>
#include <string>

namespace AIGC
{
	class JsonHelper
	{
	public:
        static std::string GetValue(std::string sStr, std::vector<std::string> sNames, std::string sDefault = "");
        static int GetValueInt(std::string sStr, std::vector<std::string> sNames, int iDefault = 0);
        static double GetValueDouble(std::string sStr, std::vector<std::string> sNames, double dDefault = 0);
        static bool GetValueBool(std::string sStr, std::vector<std::string> sNames, bool bDefault = false);
	};
}

