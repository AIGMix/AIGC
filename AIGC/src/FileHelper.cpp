#include "FileHelper.h"
#include "StringHelper.h"
#include "PathHelper.h"
#include <io.h>

namespace aigc
{

long FileHelper::GetSize(const std::string &filePath)
{
    FILE *fp = fopen(filePath.c_str(), "rb");
    if (fp == NULL)
        return 0;

    fseek(fp, 0 , SEEK_END);
    long len = ftell(fp);
    fclose(fp);
    return len;
}

bool FileHelper::IsExist(const std::string &filePath)
{
    if (access(filePath.c_str(), 0) == 0)
        return true;
    return false;
}

bool FileHelper::Copy(const std::string &filePath, const std::string &toPath)
{
    FILE *srcFP = fopen(filePath.c_str(), "rb");
    if (srcFP == NULL)
        return false;

    //创建目录
    std::string dirname = PathHelper::GetDirName(toPath);
    if (PathHelper::Mkdirs(dirname) == false)
        return false;

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

bool FileHelper::Move(const std::string &filePath, const std::string &toPath)
{
    bool success = Copy(filePath, toPath);
    if(success == false)
        return false;

    remove(filePath.c_str());
    return true;
}

bool FileHelper::Write(const std::string &filePath, const std::string &content, bool append)
{
    std::string mode = append ? "a+" : "w+";
    FILE *fp = fopen(filePath.c_str(), mode.c_str());
    if (fp == NULL)
        return false;

    fwrite(content.c_str(), 1, content.length(), fp);
    fclose(fp);
    return true;
}

bool FileHelper::WriteLines(const std::string &filePath, std::vector<std::string> &lines, bool append = false)
{
    std::string mode = append ? "a+" : "w+";
    FILE *fp = fopen(filePath.c_str(), mode.c_str());
    if (fp == NULL)
        return false;

    for (int i = 0; i < lines.size(); i++)
    {
        fwrite((lines[i] + '\n').c_str(), 1, lines[i].length() + 1, fp);
    }
    fclose(fp);
    return true;
}

std::string FileHelper::Read(const std::string &filePath)
{
    FILE *fp = fopen(filePath.c_str(), "r");
    if (fp == NULL)
        return "";

    int len;
    char buff[1024];
    std::string ret = "";
    while (1)
    {
        len = fread(buff, 1, 1024, fp);
        ret += buff;
        if (feof(fp))
            break;
    }
    fclose(fp);

    return ret;
}

std::vector<std::string> FileHelper::ReadLines(const std::string &filePath)
{
    std::string content = Read(filePath);
    std::vector<std::string> ret = StringHelper::Split(content, '\n');
    return ret;
}

}