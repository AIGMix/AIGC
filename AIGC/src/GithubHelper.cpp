#include "GithubHelper.h"
#include "HttpHelper.h"
#include "JsonHelper.h"

namespace aigc
{
    std::string GithubHelper::GetLatestVersion(const std::string &author, const std::string &repository)
    {
        std::string url = "https://api.github.com/repos/";
        url += author + '/';
        url += repository + "/releases/latest";

        HttpHelper::Response result = HttpHelper::Get(url);
        if (result.success == false)
            return "";

        std::vector<std::string> names;
        names.push_back("tag_name");
        std::string version = JsonHelper::GetValue(result.dataStr, names);
        return version;
    }
}

 //int main()
 //{
 //    std::string ver = aigc::GithubHelper::GetLatestVersion("yaronzz","Tidal-Media-Downloader");
 //    return 0;
 //}