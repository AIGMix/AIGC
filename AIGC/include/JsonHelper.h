#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <vector>
#include <string>
#include "rapidjson/document.h"
#include "StringHelper.h"

#define JSON_HELPER_LOAD_MEMBERS(...)               \
public:                                             \
    bool JsonToClass(string sjon)                   \
    {                                               \
        if (!LoadJsonString(sjon))                  \
            return false;                           \
        InitMembers(ARGS_STRING_LIST(__VA_ARGS__)); \
        return SetMember(0, __VA_ARGS__);           \
    }

namespace aigc
{
    class JsonHelper
    {
    public:
        JsonHelper(std::string jsonStr = "");
        ~JsonHelper();

        /**
         * @brief 加在json字符串
         */
        bool LoadJsonString(std::string jsonStr);

        /**
         * @brief 获取字符串类型的值
         * @param names 关键字列表，如 {"students","xiaoming","name"}
         * @param defaultV 默认值
         */
        std::string GetString(const strings<const std::string> names, std::string defaultV = "");

        /**
         * @brief 获取Int类型的值
         * @param names 关键字列表，如 {"students","xiaoming","name"}
         * @param defaultV 默认值
         */
        int GetInt(strings<const std::string> names, int defaultV = 0);

        /**
         * @brief 获取Double类型的值
         * @param names 关键字列表，如 {"students","xiaoming","name"}
         * @param defaultV 默认值
         */
        double GetDouble(strings<const std::string> names, double defaultV = 0);

        /**
         * @brief 获取Bool类型的值
         * @param names 关键字列表，如 {"students","xiaoming","name"}
         * @param defaultV 默认值
         */
        bool GetBool(strings<const std::string> names, bool defaultV = false);

        /**
         * @brief 初始化成员
         * @param names 成员字符串，ARGS_STRING_LIST(xxxx,xxxx,xxx)实现
         */
        void InitMembers(std::string names);

        /**
         * @brief 给成员赋值
         * @param index 成员的下标
         * @param arg 当前的成员
         * @param args 剩下的
         */
        template <typename T, typename... Ts>
        bool SetMember(int index, T& arg, Ts &... args);

        /**
         * @brief 给成员赋值
         * @param index 成员的下标
         * @param arg 当前的成员
         */
        template<typename T>
        bool SetMember(int index, T& arg);

    private:
        rapidjson::Document m_doc;
        std::vector<std::string> m_members;

        /**
         * @brief 获取值
         * @param names 关键字列表，如 {"students","xiaoming","name"}
         * @param value 值
         * @return True 成功
         */
        bool GetValue(const strings<const std::string> &names, rapidjson::Value &value);
        bool SetValue(int &person, rapidjson::Value &value);
        bool SetValue(bool &person, rapidjson::Value &value);
        bool SetValue(double &person, rapidjson::Value &value);
        bool SetValue(std::string &person, rapidjson::Value &value);
    };

    template <typename T>
    bool JsonHelper::SetMember(int index, T &arg)
    {
        rapidjson::Value value;
        if (!GetValue({ m_members[index] }, value))
            return false;
        return SetValue(arg, value);
    }

    template <typename T, typename... Ts>
    bool JsonHelper::SetMember(int index, T &arg, Ts &... args)
    {
        if (!SetMember(index, arg))
            return false;
        return SetMember(++index, args...);
    }
}

#endif