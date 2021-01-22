#pragma once
#ifdef _WIN32
#include <io.h>
#else
#include <sys/io.h>
#endif // _WIN32

#include <string>
#include <vector>

#include "aigc/common/StringHelper.h"

namespace aigc
{
    /******************************************************
     * 
     * 内部文件句柄
     * 
     ******************************************************/
    class AIGCFILEPrivate
    {
    public:
        AIGCFILEPrivate(FILE** fp, const std::string& filePath, const std::string& openMode)
        {
            m_fp = fopen(filePath.c_str(), openMode.c_str());
            *fp = m_fp;
        }

        ~AIGCFILEPrivate()
        {
            if (m_fp)
            {
                fclose(m_fp);
                m_fp = NULL;
            }
        }

        FILE* Handle()
        {
            return m_fp;
        }

    private:
        FILE* m_fp;
    };
    typedef std::shared_ptr<AIGCFILEPrivate> AIGCFILEPrivatePtr;

    //外部使用的文件句柄
#define AIGCFILE(filePath, openMode) \
    FILE* aigcFile = NULL; \
    for (AIGCFILEPrivatePtr AIGCFilePrivatePtr = AIGCFILEPrivatePtr(new AIGCFILEPrivate(&aigcFile, filePath, openMode)); AIGCFilePrivatePtr != NULL && aigcFile != NULL; AIGCFilePrivatePtr = NULL) 


    /******************************************************
     *
     * 提供给外部使用的工具类
     *
     ******************************************************/
    class FileHelper
    {
    public:
        /**
         * @brief 获取文件大小
         */
        static long GetSize(const std::string &filePath)
        {
            AIGCFILE(filePath, "rb")
            {
                fseek(aigcFile, 0, SEEK_END);
                return ftell(aigcFile);
            }
            return 0;
        }

        /**
         * @brief 文件是否存在
         */
        static bool IsExist(const std::string &filePath)
        {
            return access(filePath.c_str(), 0) == 0;
        }

        /**
         * @brief 复制文件
         * @param filePath 文件路径
         * @param toPath 目标路径
         */
        static bool Copy(const std::string &filePath, const std::string &toPath)
        {
            AIGCFILE(filePath, "rb")
            {
                FILE* descFP = fopen(toPath.c_str(), "wb+");
                if (descFP == NULL)
                    return false;

                char buff[1024];
                while (1)
                {
                    int len = fread(buff, 1, 1024, aigcFile);
                    len = fwrite(buff, len, 1, descFP);
                    if (feof(aigcFile))
                        break;
                }
                fclose(descFP);
                return true;
            }
            return false;
        }

        /**
         * @brief 移动文件
         * @param filePath 文件路径
         * @param toPath 目标路径
         */
        static bool Move(const std::string &filePath, const std::string &toPath)
        {
            if (!Copy(filePath, toPath))
                return false;

            remove(filePath.c_str());
            return true;
        }

        /**
         * @brief 写文件
         * @param filePath 文件路径
         * @param content 内容
         * @param append 追加
         */
        static bool Write(const std::string &filePath, const std::string &content, bool append = false)
        {
            AIGCFILE(filePath, append ? "a+" : "w+")
            {
                fwrite(content.c_str(), 1, content.length(), aigcFile);
                return true;
            }
            return false;
        }

        /**
         * @brief 写文件
         * @param filePath 文件路径
         * @param content 内容
         * @param length 内容长度
         * @param append 追加
         */
        static bool WriteByte(const std::string &filePath, const char *content, int length, bool append = false)
        {
            AIGCFILE(filePath, append ? "ab+" : "wb+")
            {
                fwrite(content, 1, length, aigcFile);
                return true;
            }
            return false;
        }

        /**
         * @brief 写文件
         * @param filePath 文件路径
         * @param lines 行集合
         * @param append 追加
         */
        static bool WriteLines(const std::string &filePath, const strings &lines, bool append = false)
        {
            AIGCFILE(filePath, append ? "a+" : "w+")
            {
                for (auto var : lines)
                {
                    fwrite((var + '\n').c_str(), 1, var.length() + 1, aigcFile);
                }
                return true;
            }
            return false;
        }

        /**
         * @brief 读文件
         * @param filePath 文件路径
         */
        static std::string Read(const std::string &filePath)
        {
            AIGCFILE(filePath, "r")
            {
                fseek(aigcFile, 0, SEEK_END);
                long size = ftell(aigcFile);
                if (size <= 0)
                    return "";

                fseek(aigcFile, 0, SEEK_SET);
                char* buff = (char*)malloc(size + 1);
                if (buff == NULL)
                    return "";

                int length = fread(buff, 1, size, aigcFile);
                buff[length] = '\0';
                std::string ret = buff;

                free(buff);
                return ret;
            }
            return "";
        }

        /**
         * @brief 读文件
         * @param filePath 文件路径
         * @param length 内容长度
         * @return 内容（需要调用free释放）
         */
        static char *ReadByte(const std::string &filePath, int &length)
        {
            AIGCFILE(filePath, "rb")
            {
                fseek(aigcFile, 0, SEEK_END);
                long size = ftell(aigcFile);
                if (size <= 0)
                    return NULL;

                fseek(aigcFile, 0, SEEK_SET);
                char* buff = (char*)malloc(size + 1);
                if (buff == NULL)
                    return NULL;

                int length = fread(buff, 1, size, aigcFile);
                return buff;
            }
            return NULL;
        }

        /**
         * @brief 读文件所有行
         * @param filePath 文件路径
         * @return 内容行数组
         */
        static strings ReadLines(const std::string &filePath)
        {
            std::string content = Read(filePath);
            strings ret = StringHelper::Split(content, '\n');
            return ret;
        }
    };

    

} // namespace aigc
