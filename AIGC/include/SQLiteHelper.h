#pragma once

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

namespace aigc
{

class SQLiteConnect
{
public:
    SQLiteConnect(std::string hostName, std::string databaseName, std::string username, std::string password, std::string path);
    ~SQLiteConnect();

    bool Open();
    bool Close();

    bool Execute(std::string sql);
    bool Execute(std::vector<std::string> sqls);
    std::string GetResult();

private:
    std::string m_hostName;
    std::string m_databaseName;
    std::string m_username;
    std::string m_password;
    std::string m_path;   
};

typedef std::shared_ptr<SQLiteConnect> SQLiteConnectPtr;

class SQLiteTable
{
public:
    struct Field
    {
        std::string name;
        std::string type;
        Field(){}
        Field(std::string name, std::string type) : name(name), type(type){}
    };

    SQLiteTable(std::string tableName, SQLiteConnectPtr sqlConnect);

    void AddField(std::string name, std::string type);
    bool CreatTable();
    bool DeleteField();
    bool IsExist();

    int GetRowCount();
    int AddRow(std::vector<std::string> sqls);
    int InsertRow(int index, std::vector<std::string> sqls);
    int RemoveRow(int index);
    std::vector<std::string> GetRow(int index, int count);
    std::vector<std::string> GetRow(std::string condition, int count);

private:
    std::string m_tableName;
    std::list<Field> m_fields;
}


} 
