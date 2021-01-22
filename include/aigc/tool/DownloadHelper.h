#pragma once

#include "aigc/common/FileHelper.h"
#include "aigc/net/HttpRequestHelper.h"

namespace aigc
{
    class DownloadHelper
    {
    public:
        /**
         * @brief 获取文件大小
         * @param url 链接
         */
        static int GetFileSize(const std::string& url)
        {
			HttpRequestHelperPtr request = std::make_shared<HttpRequestHelper>(url, "HEAD");
			if (!request->SendRequest())
                return -1;

            HttpResponseHelperPtr response = request->GetResponse();
            if (response == NULL)
                return -1;

            return response->contentLength;
        }

		/**
		 * @brief 回调接口：更新回调 | 完成回调 | 错误回调
		 * @param totalSize 文件大小
		 * @param downloadSize 已下载大小
		 * @param increSize 增长大小
		 * @param message 错误信息
		 */
		typedef std::function<void(int totalSize, int downloadSize, int increSize)> IncreaseNotify;
		typedef std::function<void(int totalSize)> CompleteNotify;
		typedef std::function<void(std::string message)> ErrorNotify;

        
        /**
         * @brief 下载文件
         * @param url 链接
         * @param path 路径
         * @param increaseFunc 更新回调 
         * @param completeFunc 完成回调
         * @param errorFunc 错误回调
         * @param bufferSize 缓存大小
         */
        static bool Download(const std::string& url, 
                             const std::string path,
                            IncreaseNotify increaseFunc = NULL,
                            CompleteNotify completeFunc = NULL,
                            ErrorNotify errorFunc = NULL,
                            int bufferSize = 1024)
        {
			HttpRequestHelperPtr request = std::make_shared<HttpRequestHelper>(url, "GET");
            if (!request->SendRequest())
            {
                if (errorFunc)
                    errorFunc(request->message);
                return false;
            }

			HttpResponseHelperPtr response = request->GetResponse();
			if (response == NULL)
			{
				if (errorFunc)
					errorFunc(request->message);
				return false;
			}

            int currentSize = 0;
            if (bufferSize < 0)
                bufferSize = 1024;
            AIGCFILE(path, "wb+")
            {
                std::shared_ptr<char> pbuffer = StringHelper::Malloc(bufferSize);
                char* buffer = pbuffer.get();
				while (true)
				{
                    int size = response->Read(buffer, bufferSize);
                    if (size < 0)
					{
						if (errorFunc)
							errorFunc(response->message);
						return false;
					}
                    if (size == 0)
                    {
                        if (completeFunc)
                            completeFunc(response->contentLength);
                        return true;
                    }

                    fwrite(buffer, size, 1, aigcFile);
                    currentSize += size;
                    if (increaseFunc)
                        increaseFunc(response->contentLength, currentSize, size);
				}
            }

			if (errorFunc)
				errorFunc("Open file failed!");
            return false;
        }
    };

} // namespace aigc
