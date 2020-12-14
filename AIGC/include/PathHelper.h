#pragma once

#include <io.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>

#include "StringHelper.h"
namespace aigc
{
    class PathHelper
    {
    public:
        /**
         * @brief 获取目录大小
         * @param path 路径
         */
        static long GetSize(const std::string& path)
        {
            long ret = 0;
            strings files = GetAllFiles(path);

            for(auto var : files)
            {
                FILE *fp = fopen(var.c_str(), "rb");
                if (fp != NULL)
                {
                    fseek(fp, 0, SEEK_END);
                    ret += ftell(fp);
                    fclose(fp);
                }
            }
            return ret;
        }

        /**
         * @brief 是否为绝对路径
         * @param path 路径
         */
        static bool IsFullPath(const std::string &path)
        {
            /**
             * 样例 1、g:/  2、g:\  3、\\192.168.0.1   4、/root
             */
            if (path.find(":/") == 1 || path.find(":\\") == 1 || path.find("\\\\") == 0 || path.find("/") == 0)
                return true;
            return false;
        }

        /**
         * @brief 获取绝对路径
         * @param path 相对路径
         */
        static std::string GetFullPath(const std::string& path)
        {
            std::string filename = GetFileName(path);
            std::string dirname = GetDirName(path);

            std::vector<std::string> paths = StringHelper::Split(dirname, '/');
            if (IsFullPath(dirname) == false)
            {
                std::string pwd = StringHelper::Replace(GetWorkPath(), "\\", "/");
                std::vector<std::string> pwdPaths = StringHelper::Split(pwd, '/');
                paths.insert(paths.begin(), pwdPaths.begin(), pwdPaths.end());
            }

            /*遍历查找 .和..*/
            int num = (int)paths.size();
            for (int i = num - 1; i >= 0; i--)
            {
                if (paths[i] == ".")
                    paths.erase(paths.begin() + i);
                if (paths[i] == "..")
                {
                    paths.erase(paths.begin() + i);
                    i--;
                    if (i >= 0)
                        paths.erase(paths.begin() + i);
                }
            }

            std::string newPaths = "";
            for (int i = 0; i < paths.size(); i++)
            {
                newPaths += paths[i] + '/';
            }
            return newPaths + filename;
        }

        /**
         * @brief 移除目录限制字符
         * @param path 路径
         * @param newChar 替换的字符
         * @return 信的字符串
         */
        static std::string ReplaceLimitChar(const std::string &path, const std::string &newChar = " ")
        {
            std::string ret;
            ret = StringHelper::Replace(path, ":", newChar);
            ret = StringHelper::Replace(ret, "/", newChar);
            ret = StringHelper::Replace(ret, "?", newChar);
            ret = StringHelper::Replace(ret, "<", newChar);
            ret = StringHelper::Replace(ret, ">", newChar);
            ret = StringHelper::Replace(ret, "|", newChar);
            ret = StringHelper::Replace(ret, "\\", newChar);
            ret = StringHelper::Replace(ret, "*", newChar);
            ret = StringHelper::Replace(ret, "\"", newChar);
            return path;
        }

        /**
         * @brief 获取目录下的所有文件
         * @param path 路径
         * @return 文件名路径数组
         */
        static strings GetAllFiles(const std::string &path)
        {
            /*文件句柄和文件信息*/
            long fileHandle = 0;
            struct _finddata_t fileinfo;
            std::string tmpPath;
            strings ret;

            if ((fileHandle = _findfirst(tmpPath.assign(path).append("/*").c_str(), &fileinfo)) == -1)
                return ret;

            do
            {
                /*判断是否为子目录*/
                if ((fileinfo.attrib & _A_SUBDIR))
                {
                    if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    {
                        // ret.push_back(tmpPath.assign(path).append("/").append(fileinfo.name));
                        /*递归搜索*/
                        strings subfiles = GetAllFiles(tmpPath.assign(path).append("/").append(fileinfo.name));
                        ret.insert(ret.end(), subfiles.begin(), subfiles.end());
                    }
                }
                else
                {
                    ret.push_back(tmpPath.assign(path).append("/").append(fileinfo.name));
                }

            } while (_findnext(fileHandle, &fileinfo) == 0);

            /*关闭句柄*/
            _findclose(fileHandle);
            return ret;
        }

        /**
         * @brief 新建多层目录
         * @param path 路径
         * @return True 成功
         */
        static bool Mkdirs(const std::string& path)
        {
            if (access(path.c_str(), 0) == 0)
                return true;

            std::string tmppath = StringHelper::Replace(path, "\\", "/");

            //先递归创建前面的目录
            //将‘//’的分隔符改成‘/'
            tmppath = StringHelper::Replace(path, "//", "/");

            /*最后一个符号如果是'/'要去掉，不然下面的步骤会出问题*/
            if (tmppath[tmppath.size() - 1] == '/')
                tmppath = tmppath.substr(0, tmppath.size() - 1);

            std::string::size_type pos = tmppath.find_last_of("/");
            if (pos != std::string::npos)
            {
                tmppath = tmppath.substr(0, pos) + '/';
                if (Mkdirs(tmppath) == false)
                    return false;
            }

            if (mkdir(path.c_str()) != 0)
                return false;
            return true;
        }

        /**
         * @brief 删除文件夹
         * @param path 路径
         * @return True 成功
         */
        static bool Remove(const std::string& path)
        {
            if (access(path.c_str(), 0) != 0)
                return true;

            /*文件句柄和文件信息*/
            long fileHandle = 0;
            struct _finddata_t fileinfo;
            std::string tmpPath;
            std::vector<std::string> ret;

            if ((fileHandle = _findfirst(tmpPath.assign(path).append("/*").c_str(), &fileinfo)) != -1)
            {
                do
                {
                    /*判断是否为子目录*/
                    if ((fileinfo.attrib & _A_SUBDIR))
                    {
                        if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                        {
                            /*递归删除*/
                            Remove(tmpPath.assign(path).append("/").append(fileinfo.name));
                        }
                    }
                    else
                    {
                        remove(tmpPath.assign(path).append("/").append(fileinfo.name).c_str());
                    }

                } while (_findnext(fileHandle, &fileinfo) == 0);

                /*关闭句柄*/
                _findclose(fileHandle);
            }
            return rmdir(path.c_str()) == 0;
        }

        /**
         * @brief 复制目录
         * @param path 路径
         * @param toPath 新路径
         */
        static bool Copy(const std::string &path, const std::string &toPath)
        {
            long ret = 0;

            std::string from = GetFullPath(path);
            std::string to = GetFullPath(toPath);
            
            strings files = GetAllFiles(path);
            for (int i = 0; i < files.size(); i++)
            {
                bool success = false;
                std::string filepath = StringHelper::Replace(files[i], "\\", "/");
                std::string newpath = StringHelper::Replace(files[i], from, to);
                //复制
                FILE *srcFP = fopen(filepath.c_str(), "rb");
                FILE *descFP = fopen(toPath.c_str(), "wb+");
                if (srcFP != NULL && descFP != NULL)
                {
                    //创建目录
                    std::string dirname = GetDirName(toPath);
                    if (Mkdirs(dirname))
                    {
                        char buff[1024];
                        while (1)
                        {
                            int len = fread(buff, 1, 1024, srcFP);
                            len = fwrite(buff, len, 1, descFP);
                            if (feof(srcFP))
                                break;
                        }
                        success = true;
                    }
                }
                if (srcFP != NULL)
                    fclose(srcFP);
                if (descFP != NULL)
                    fclose(descFP);

                if (!success)
                    return false;
            }
            return true;
        }

        /**
         * @brief 移动目录
         * @param path 路径
         * @param toPath 新路径
         */
        static bool Move(const std::string &path, const std::string &toPath)
        {
            if (!Copy(path, toPath))
                return false;
            if (!Remove(path))
                return false;
            return true;
        }

        /**
         * @brief 获取当前工作路径
         * @return 路径
         */
        static std::string GetWorkPath()
        {
            char buffer[1024];
            char *ret = getcwd(buffer, 1024);
            return ret == NULL ? "" : std::string(ret);
        }

    public:
        typedef struct PathInfoStruct
        {
            std::string FileName;
            std::string FileNameNoExt;
            std::string FileExtension;
            std::string DirName;
        }PathInfo;

        static PathInfo GetPathInfo(const std::string &path)
        {
            std::string strPath = StringHelper::Replace(path, "\\", "/");
            std::string::size_type pos = strPath.find_last_of("/");

            std::string dirName = "./";
            if (pos != std::string::npos)
                dirName = strPath.substr(0, pos) + '/';

            std::string fileName = path;
            if (dirName != "./")
                fileName = StringHelper::Replace(strPath, dirName, "");

            std::string fileNameNoExt = fileName;
            std::string fileExtension = "";
            pos = fileName.find_last_of('.');
            if (pos != std::string::npos)
            {
                fileNameNoExt = fileName.substr(0, pos);
                fileExtension = fileName.substr(pos + 1);
            }

            PathInfo info;
            info.FileExtension = fileExtension;
            info.FileName = fileName;
            info.FileNameNoExt = fileNameNoExt;
            info.DirName = dirName;
            return info;
        }

        /**
         * @brief 获取路径 eg. /root/home/test.json -> /root/home/
         */
        static std::string GetDirName(const std::string& path)
        {
            PathInfo info = GetPathInfo(path);
            return info.DirName;
        }

        /**
         * @brief 获取文件名 eg. /root/home/test.json -> test.json
         */
        static std::string GetFileName(const std::string& path)
        {
            PathInfo info = GetPathInfo(path);
            return info.FileName;
        }

        /**
         * @brief 获取文件名(没带扩展) eg. /root/home/test.json -> test
         */
        static std::string GetFileNameWithoutExt(const std::string& path)
        {
            PathInfo info = GetPathInfo(path);
            return info.FileNameNoExt;
        }

        /**
         * @brief 获取扩展名 eg. /root/home/test.json -> json
         */
        static std::string GetFileExtension(const std::string &path)
        {
            PathInfo info = GetPathInfo(path);
            return info.FileExtension;
        }
    };

} 
