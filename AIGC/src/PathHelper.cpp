#include "PathHelper.h"
#include "StringHelper.h"
#include "FileHelper.h"

#include <direct.h>
#include <io.h>
#include <string.h>
#include <iostream>

namespace aigc
{
    long PathHelper::GetSize(const std::string &path)
    {
        long ret = 0;
        std::vector<std::string> files = GetAllFiles(path);
        for (int i = 0; i < files.size(); i++)
        {
            long size = FileHelper::GetSize(files[i]);
            ret += size;
            std::cout << "[" + std::to_string(size) + "] " + files[i] << std::endl;
        }
        return ret;
    }

    bool PathHelper::IsAbsolutePath(const std::string &path)
    {
        if (path.find(":/") == 1 || path.find(":\\") == 1 || path.find("\\\\") == 0 || path.find("/") == 0)
            return true;
        return false;
    }

    std::string PathHelper::GetFullPath(const std::string &path)
    {
        std::string filename = GetFileName(path);
        std::string dirname = GetDirName(path);

        std::vector<std::string> paths = StringHelper::Split(dirname, '/');
        if (IsAbsolutePath(dirname) == false)
        {
            std::string pwd = StringHelper::Replace(GetWorkPath(), "\\", "/");
            std::vector<std::string> pwdPaths = StringHelper::Split(pwd, '/');
            paths.insert(paths.begin(), pwdPaths.begin(), pwdPaths.end());
        }

        //遍历查找 .和..
        int num = paths.size();
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

    std::string PathHelper::ReplaceLimitChar(const std::string &path, const std::string &newChar)
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

    std::vector<std::string> PathHelper::GetAllFiles(const std::string &path)
    {
        //文件句柄和文件信息
        long fileHandle = 0;
        struct _finddata_t fileinfo;
        std::string tmpPath;
        std::vector<std::string> ret;

        if ((fileHandle = _findfirst(tmpPath.assign(path).append("/*").c_str(), &fileinfo)) == -1)
            return ret;

        do
        {
            //判断是否为子目录
            if ((fileinfo.attrib & _A_SUBDIR))
            { 
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                {
                    // ret.push_back(tmpPath.assign(path).append("/").append(fileinfo.name));
                    //递归搜索
                    std::vector<std::string> subfiles = GetAllFiles(tmpPath.assign(path).append("/").append(fileinfo.name));
                    ret.insert(ret.end(), subfiles.begin(), subfiles.end());
                }
            }
            else
            {
                ret.push_back(tmpPath.assign(path).append("/").append(fileinfo.name));
            }

        } while (_findnext(fileHandle, &fileinfo) == 0); 

        //关闭句柄
        // _findclose(fileHandle);
        return ret;
    }

    bool PathHelper::Mkdirs(const std::string &path)
    {
        if (access(path.c_str(), 0) == 0)
            return true;

        //先递归创建前面的目录
        std::string tmppath = StringHelper::Replace(path, "\\", "/");

        //将‘//’的分隔符改成‘/'
        tmppath = StringHelper::Replace(path, "//", "/");
        //最后一个符号如果是'/'要去掉，不然下面的步骤会出问题
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

    bool PathHelper::Remove(const std::string &path)
    {
        if (access(path.c_str(), 0) != 0)
            return true;

        //文件句柄和文件信息
        long fileHandle = 0;
        struct _finddata_t fileinfo;
        std::string tmpPath;
        std::vector<std::string> ret;

        if ((fileHandle = _findfirst(tmpPath.assign(path).append("/*").c_str(), &fileinfo)) != -1)
        {
            do
            {
                //判断是否为子目录
                if ((fileinfo.attrib & _A_SUBDIR))
                {
                    if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    {
                        //递归删除
                        Remove(tmpPath.assign(path).append("/").append(fileinfo.name));
                    }
                }
                else
                {
                    remove(tmpPath.assign(path).append("/").append(fileinfo.name).c_str());
                }

            } while (_findnext(fileHandle, &fileinfo) == 0);

            //关闭句柄
            // _findclose(fileHandle);
        }
        return rmdir(path.c_str()) == 0;
    }

    bool PathHelper::Copy(const std::string& path, const std::string& toPath)
    {
        long ret = 0;

        std::string from = GetFullPath(path);
        std::string to = GetFullPath(toPath);
        std::vector<std::string> files = GetAllFiles(path);
        for (int i = 0; i < files.size(); i++)
        {
            std::string filepath = StringHelper::Replace(files[i], "\\", "/");
            std::string newpath = StringHelper::Replace(files[i], from, to);
            if (FileHelper::Copy(filepath, newpath) == false)
                return false;
        }
        return true;
    }

    bool PathHelper::Move(const std::string &path, const std::string &toPath)
    {
        if (Copy(path, toPath) == false)
            return false;
        if (Remove(path) == false)
            return false;
        return true;
    }

    std::string PathHelper::GetWorkPath()
    {
        char buffer[1024];
        char* ret = getcwd(buffer, 1024);
        return std::string(ret);
    }

    std::string PathHelper::GetDirName(const std::string &path)
    {
        std::string strPath = StringHelper::Replace(path, "\\","/");
        std::string::size_type pos = strPath.find_last_of("/");

        if (pos == std::string::npos)
            return "./";

        strPath = strPath.substr(0, pos) + '/';
        return strPath;
    }

    std::string PathHelper::GetFileName(const std::string &path)
    {
        std::string strPath = StringHelper::Replace(path, "\\", "/");
        std::string dirpath = GetDirName(path);
        if (dirpath == "./")
            return path;

        std::string ret = StringHelper::Replace(strPath, dirpath, "");        
        return ret;
    }

    std::string PathHelper::GetFileNameWithoutExt(const std::string &path)
    {
        std::string filename = GetFileName(path);
        std::string::size_type pos = filename.find_last_of('.');
        if (pos == std::string::npos)
            return filename;

        std::string ret = filename.substr(0, pos);
        return ret;
    }

    std::string PathHelper::GetFielExt(const std::string &path)
    {
        std::string filename = GetFileName(path);
        std::string::size_type pos = filename.find_last_of('.');
        if (pos == std::string::npos)
            return "";

        std::string ret = filename.substr(pos + 1);
        return ret;
    }

}

// int main()
// {
//     std::string ret = aigc::PathHelper::GetWorkPath();
//     std::string name = "C:\\favor\\my\\test.json";
//     ret = aigc::PathHelper::GetDirName(name);
//     ret = aigc::PathHelper::GetFileName(name);
//     ret = aigc::PathHelper::GetFileNameWithoutExt(name);
//     ret = aigc::PathHelper::GetFielExt(name);

//     std::string name2 = "../my/test.json";
//     bool flag = aigc::PathHelper::IsAbsolutePath(name2);
//     ret = aigc::PathHelper::GetFullPath(name2);

//     std::string name3 = "D:/code/Common/AIGPY/";
//     std::string name4 = "D:/code/Common/AIGPY2/";
//     std::vector<std::string> files = aigc::PathHelper::GetAllFiles(name3);
//     for (size_t i = 0; i < files.size(); i++)
//     {
//         std::cout<< files[i] << std::endl;
//     }
//     std::cout<< files.size() << std::endl;

//     long size = aigc::PathHelper::GetSize(name3);
//     flag = aigc::PathHelper::Copy(name3, name4);
//     flag = aigc::PathHelper::Remove(name4);

//     return 0;
// }
