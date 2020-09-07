/**********
* Date    :   2020/09/07
* Author  :   Yaronzz
* Contact :   yaronhuang@foxmail.com
* Desc    :   
**********/
#pragma once

#include <string>
#include <vector>

namespace aigc
{

class FileHelper
{
public:
    /**
     * @brief 获取文件大小
     * @param {string} filePath 文件路径
     * @return {long} 大小
     */
    static long GetSize(const std::string& filePath);

    /**
     * @brief 文件是否存在
     * @param {string} filePath 文件路径
     * @return {bool} 是否
     */
    static bool IsExist(const std::string& filePath);

    /**
     * @brief 复制文件
     * @param {string} filePath 文件路径
     * @param {string} toPath 目标路径
     * @return {bool} 
     */
    static bool Copy(const std::string& filePath, const std::string& toPath);

    /**
     * @brief 移动文件
     * @param {string} filePath 文件路径
     * @param {string} toPath 目标路径
     * @return {bool} 
     */
    static bool Move(const std::string& filePath, const std::string& toPath);

    /**
     * @brief 写文件
     * @param {string} filePath 文件路径
     * @param {string} content 内容
     * @param {bool} append 追加
     * @return {bool} 
     */
    static bool Write(const std::string& filePath, const std::string& content, bool append = false);

    /**
     * @brief 读文件
     * @param {string} filePath 文件路径
     * @return {string} 内容
     */
    static std::string Read(const std::string &filePath);

    /**
     * @brief 读文件所有行
     * @param {string} filePath 文件路径
     * @return {vector} 内容
     */
    static std::vector<std::string> ReadLines(const std::string &filePath);
};

} 
