#pragma once

#include <string>
#include <vector>

namespace aigc
{

class M3u8Helper
{
public:
    static std::vector<std::string> GetTSUrls(const std::string &url);
    static long GetSize(const std::string& url);
    static bool DownloadFile(const std::string &url, const std::string &toPath);
};

} 
