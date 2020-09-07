#pragma once

#include <string>
#include <vector>

namespace aigc
{

class GithubHelper
{
public:
    static std::string GetLatestVersion(const std::string &author, const std::string &repository);
    static std::string DownloadFile(const std::string &author, 
                                    const std::string &repository, 
                                    const std::string &version, 
                                    const std::string &fileName, 
                                    const std::string &savePath);
};

} 
