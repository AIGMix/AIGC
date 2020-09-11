#pragma once

#include <string>
#include <vector>
#include <map>

namespace aigc
{
template<typename TVALUE>
class HashHelper
{
public:
    /**
     * @brief 构造函数
     * @param capacity 哈希表数组的容量
     */
    HashHelper(int capacity = 100);
    ~HashHelper();

    /**
     * @brief 添加
     * @param key 关键字
     * @param value 值
     * @return True 成功
     */
    bool Add(std::string key, TVALUE value);

    /**
     * @brief 删除
     * @param key 关键字
     * @return True 成功
     */
    bool Remove(std::string key);

    /**
     * @brief 删除全部
     */
    bool RemoveAll();

    /**
     * @brief 获取值
     * @param key 关键字
     * @param defaultValue 默认值
     */
    TVALUE Get(std::string key, TVALUE defaultValue);

    /**
     * @brief 获取值
     * @param key 关键字
     * @note 如果没有此关键字会报错
     */
    TVALUE &operator[](std::string key);

    /**
     * @brief 查询是否存在此关键字
     * @param key 关键字
     */
    bool IsExist(std::string key);

private:
    typedef struct NodeStruct
    {
        int hash;               //哈希值
        std::string key;        //关键字
        TVALUE value;           //值
        struct NodeStruct *next;//下一个节点
    }Node;

    /**
     * @brief 计算哈希表数组的容量
     * @param capacity 输入的容量
     * @param cmpCapacity 比较容量，如果插入的节点超过此值，就要扩展容量
     * @return 计算后的容量 
     */
    int CalcCapacity(int capacity, int& cmpCapacity);

    /**
     * @brief 计算关键字的哈希值
     * @param key 关键字
     */
    int CalcHash(std::string key);

    /**
     * @brief 计算关键字在哈希表数组中的下标位置
     * @param hash 哈希值
     */
    int CalcSeatIndex(int hash);

    /**
     * @brief 扩充哈希表数组的容量，如果需要的话
     */
    void ExpandIfNecessary();

    /**
     * @brief 新建节点
     * @param hash 哈希值
     * @param key 关键字
     * @param value 值
     * @param node 创建后的节点
     */
    bool CreatNode(int hash, std::string key, TVALUE value, Node** node);

    bool GetNode(std::string key, Node** node);

    /**
     * @brief 释放节点内存
     * @param node 节点
     */
    void RemoveNode(Node* node);


    int m_capacity;     //哈希表数组容量
    int m_cmpCapacity;  //比较容量，如果插入的节点超过此值，就要扩展容量
    int m_count;        //当前插入的节点数
    Node** m_array;     //哈希表数组
};

} // namespace aigc
