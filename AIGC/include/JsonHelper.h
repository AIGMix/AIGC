#pragma once

#include <vector>
#include <string>

namespace aigc
{
class JsonHelper
{
public:
    static std::string GetValue(std::string str, std::vector<std::string> names, std::string defaultValue = "");
    static int GetValueInt(std::string str, std::vector<std::string> names, int defaultValue = 0);
    static double GetValueDouble(std::string str, std::vector<std::string> names, double defaultValue = 0);
    static bool GetValueBool(std::string str, std::vector<std::string> names, bool defaultValue = false);
};
}

