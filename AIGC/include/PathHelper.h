#pragma once

#include <string>
#include <vector>

namespace aigc
{

class PathHelper
{
public:
    static long GetSize(const std::string& path);
    static std::string GetFullPath(const std::string& path);
    static std::string ReplaceLimitChar(const std::string& path, char newChar);
    static std::vector<std::string> GetAllFiles(const std::string& path);
    static bool Mkdirs(const std::string& path);
    static bool Remove(const std::string& path);
    static bool Copy(const std::string& path, const std::string& toPath);

    static std::string GetDirName(const std::string& path);
    static std::string GetFileName(const std::string& path);
    static std::string GetFileNameWithoutExt(const std::string& path);
    static std::string GetFielExt(const std::string &path);
};

} 
