#pragma once

#include <string>
#include <vector>

namespace aigc
{

class GithubHelper
{
public:
    /**
     * @brief 获取最新的版本号
     * @param author 作者
     * @param repository 仓库名
     * @return string 版本号
     */
    static std::string GetLatestVersion(const std::string &author, const std::string &repository);

    /**
     * @brief 下载文件
     * @param author 作者
     * @param repository 仓库名
     * @param version 版本号
     * @param fileName release上的文件
     * @param savePath 保存的路径名
     */
    static bool DownloadFile(const std::string &author,
                             const std::string &repository,
                             const std::string &version,
                             const std::string &fileName,
                             const std::string &savePath);
};

} 
