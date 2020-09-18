#include "LogHelper.h"
#include "TimeHelper.h"
#include "FileHelper.h"
#include "StringHelper.h"

namespace aigc
{
    std::string LogHelper::GetTypeString(Type type)
    {
        switch (type)
        {
        case Type::Info:   return "[INFO]:  ";
        case Type::Warn:   return "[WARN]:  ";
        case Type::Error:  return "[ERROR]: ";
        case Type::Debug:  return "[DEBUG]: ";
        case Type::Notice: return "[NOTICE]:";
        }
        return "[NOTICE]:";
    }

    LogHelper::LogHelper(std::string filePath, int fileNum, int fileMaxSize)
    {
        if (fileNum <= 0)
            fileNum = 1;

        // TODO

        //循环查询不存在的文件
        std::string choseFile = filePath;
        for (int i = 0; i < fileNum; i++)
        {
            if (i > 0)
            {
                choseFile = filePath + StringHelper::ShiftLeft(std::to_string(i), 2);
            }
            if (!FileHelper::IsExist(choseFile))
            {
                Open(choseFile);
                return;
            }
        }
        
    }

    LogHelper::~LogHelper()
    {
        Close();
    }

    bool LogHelper::Open(std::string path)
    {
        std::lock_guard<std::mutex> mutx(m_mutex);
        if (m_fp)
            Close();

        m_fp = fopen(path.c_str(), "a+");
        return m_fp != NULL;
    }

    void LogHelper::Close()
    {
        std::lock_guard<std::mutex> mutx(m_mutex);
        if (m_fp != NULL)
        {
            fclose(m_fp);
            m_fp = NULL;
        }
    }

    bool LogHelper::Write(Type type, const std::string &str)
    {
        std::lock_guard<std::mutex> mutx(m_mutex);
        if (m_fp == NULL)
            return false;

        //2020-08-26 22:30:17 [DEBUG]: xxxxxxxxxxxxxxxxx
        std::string time = TimeHelper::CurDateTime();
        std::string typeStr = GetTypeString(type);
        std::string content = time + ' ' + typeStr + ' ' + str;

        fwrite(content.c_str(), 1, content.size(), m_fp);
        return true;
    }

}