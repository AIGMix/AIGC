#pragma once

#include <string>
#include <iostream>
#include <functional>

#include "aigc/common/JsonHelper.h"
#include "aigc/net/HttpHelper.h"

namespace aigc
{

class GithubHelper
{
public:
    /**
     * @brief 下载进度回调
     * @param totalSize 文件字节数
     * @param currentSize 已下载字节数
     * @return bool False则终止下载
     */
    typedef std::function<bool(const int totalSize, const int currentSize)> UpdateDownloadNotify;

    /**
     * @brief 获取最新的版本号
     * @param author 作者
     * @param repository 仓库名
     * @return string 版本号
     * https://api.github.com/repos/yaronzz/Tidal-Media-Downloader-PRO/releases/latest
     */
    static std::string GetLatestVersion(const std::string &author, const std::string &repository)
    {
        std::string url = "https://192.30.255.117:443/repos/" + author + '/' + repository + "/releases/latest";

        HttpHelper::Response result = HttpHelper::Get(url);
        if (!result.success)
            return "";

        //std::string version = JsonHelper::GetObjectByJson<std::string>(result.dataStr, {"tag_name"});
        //return version;
    }

    /**
     * @brief 下载文件
     * @param author 作者
     * @param repository 仓库名
     * @param version 版本号
     * @param onlineFileName release上的文件
     * @param savePath 保存的路径名
     * @param updateCallBack 下载进度回调
     */
    static bool DownloadFile(const std::string &author,
                             const std::string &repository,
                             const std::string &version,
                             const std::string &onlineFileName,
                             const std::string &savePath,
                             UpdateDownloadNotify updateCallBack = NULL);
};

} 
