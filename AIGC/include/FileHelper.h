#pragma once

#include <io.h>
#include <string>
#include <vector>

#include "StringHelper.h"
#include "PathHelper.h"

namespace aigc
{
    class FileHelper
    {
    public:
        /**
         * @brief 获取文件大小
         */
        static long GetSize(const std::string &filePath)
        {
            FILE *fp = fopen(filePath.c_str(), "rb");
            if (fp == NULL)
                return 0;
            fseek(fp, 0, SEEK_END);

            long len = ftell(fp);
            fclose(fp);
            return len;
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
            FILE *srcFP = fopen(filePath.c_str(), "rb");
            if (srcFP == NULL)
                return false;

            //创建目录
            std::string dirname = PathHelper::GetDirName(toPath);
            if (PathHelper::Mkdirs(dirname) == false)
            {
                fclose(srcFP);
                return false;
            }

            FILE *descFP = fopen(toPath.c_str(), "wb+");
            if (descFP == NULL)
            {
                fclose(srcFP);
                return false;
            }

            int len;
            char buff[1024];
            while (1)
            {
                len = fread(buff, 1, 1024, srcFP);
                len = fwrite(buff, len, 1, descFP);
                if (feof(srcFP))
                    break;
            }

            fclose(srcFP);
            fclose(descFP);
            return true;
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
            FILE *fp = fopen(filePath.c_str(), append ? "a+" : "w+");
            if (fp == NULL)
                return false;

            fwrite(content.c_str(), 1, content.length(), fp);
            fclose(fp);
            return true;
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
            FILE *fp = fopen(filePath.c_str(), append ? "ab+" : "wb+");
            if (fp == NULL)
                return false;

            fwrite(content, 1, length, fp);
            fclose(fp);
            return true;
        }

        /**
         * @brief 写文件
         * @param filePath 文件路径
         * @param lines 行集合
         * @param append 追加
         */
        static bool WriteLines(const std::string &filePath, const strings &lines, bool append = false)
        {
            FILE *fp = fopen(filePath.c_str(), append ? "a+" : "w+");
            if (fp == NULL)
                return false;

            for(auto var : lines)
            {
                fwrite((var + '\n').c_str(), 1, var.length() + 1, fp);
            }
            fclose(fp);
            return true;
        }

        /**
         * @brief 读文件
         * @param filePath 文件路径
         */
        static std::string Read(const std::string &filePath)
        {
            FILE *fp = fopen(filePath.c_str(), "r");
            if (fp == NULL)
                return "";

            char buff[1024];
            std::string ret = "";
            while (1)
            {
                int len = fread(buff, 1, 1024, fp);
                ret += buff;
                if (feof(fp))
                    break;
            }
            fclose(fp);
            return ret;
        }

        /**
         * @brief 读文件
         * @param filePath 文件路径
         * @param length 内容长度
         * @return 内容（需要调用free释放）
         */
        static char *ReadByte(const std::string &filePath, int &length)
        {
            long size = GetSize(filePath);
            if (size <= 0)
                return NULL;

            FILE *fp = fopen(filePath.c_str(), "rb");
            if (fp == NULL)
                return NULL;

            char *buff = (char *)malloc(size);
            if (buff == NULL)
            {
                fclose(fp);
                return NULL;
            }

            length = fread(buff, 1, size, fp);
            fclose(fp);
            return buff;
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
