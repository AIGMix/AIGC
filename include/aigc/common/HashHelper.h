#pragma once

#include <string>
#include <vector>
#include <map>

namespace aigc
{
    template <typename T>
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
        bool Add(std::string key, T value);

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
        T Get(std::string key, T defaultValue);

        /**
         * @brief 获取值
         * @param key 关键字
         * @note 如果没有此关键字会报错
         */
        T &operator[](std::string key);

        /**
         * @brief 查询是否存在此关键字
         * @param key 关键字
         */
        bool IsExist(std::string key);

    private:
        typedef struct NodeStruct
        {
            int hash;                //哈希值
            std::string key;         //关键字
            T value;                 //值
            struct NodeStruct *next; //下一个节点
        } Node;

        /**
         * @brief 计算哈希表数组的容量
         * @param capacity 输入的容量
         * @param cmpCapacity 比较容量，如果插入的节点超过此值，就要扩展容量
         * @return 计算后的容量 
         */
        int CalcCapacity(int capacity, int &cmpCapacity);

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
        bool CreatNode(int hash, std::string key, T value, Node **node);

        /**
         * @brief 获取节点
         * @param key 关键字
         * @param node 节点
         */
        bool GetNode(std::string key, Node **node);

        /**
         * @brief 释放节点内存
         * @param node 节点
         */
        void RemoveNode(Node *node);

        int m_capacity;    //哈希表数组容量
        int m_cmpCapacity; //比较容量，如果插入的节点超过此值，就要扩展容量
        int m_count;       //当前插入的节点数
        Node **m_array;    //哈希表数组
    };

    template <typename T>
    HashHelper<T>::HashHelper(int capacity)
    {
        m_count = 0;
        m_capacity = CalcCapacity(capacity, m_cmpCapacity);

        int size = sizeof(Node *) * m_capacity;
        m_array = (Node **)malloc(size);
        if (m_array == NULL)
        {
            m_capacity = 0;
            m_cmpCapacity = 0;
        }

        memset(m_array, 0, size);
    }

    template <typename T>
    HashHelper<T>::~HashHelper()
    {
        m_count = 0;
        m_capacity = 0;
        m_cmpCapacity = 0;
        if (m_array != NULL)
        {
            RemoveAll();
            free(m_array);
            m_array = NULL;
        }
    }

    template <typename T>
    bool HashHelper<T>::Add(std::string key, T value)
    {
        //查找节点，如果存在则覆盖值
        Node *find = NULL;
        if (GetNode(key, &find) == true)
        {
            find->value = value;
            return true;
        }

        //扩充容量
        ExpandIfNecessary();

        //计算哈希值和下标
        int hash = CalcHash(key);
        int index = CalcSeatIndex(hash);

        //在链表上移动至链表尾
        find = m_array[index];
        while (find != NULL)
        {
            if (find->key == key)
                return false;
            if (find->next == NULL)
                break;
            find = find->next;
        }

        //新建节点
        Node *node = NULL;
        if (CreatNode(hash, key, value, &node) == false)
            return false;

        if (find != NULL)
            find->next = node;
        else
            m_array[index] = node;

        m_count++;
        return true;
    }

    template <typename T>
    bool HashHelper<T>::Remove(std::string key)
    {
        int hash = CalcHash(key);
        int index = CalcSeatIndex(hash);

        Node *find = m_array[index];
        if (find == NULL)
            return true;

        Node *pre = find;
        while (find != NULL)
        {
            if (find->key != key)
            {
                pre = find;
                find = find->next;
                continue;
            }

            if (find == m_array[index])
                m_array[index] = find->next;
            else
                pre->next = find->next;

            RemoveNode(find);
            m_count--;
            return true;
        }
        return true;
    }

    template <typename T>
    bool HashHelper<T>::RemoveAll()
    {
        for (int i = 0; i < m_capacity; i++)
        {
            Node *node = m_array[i];
            while (node != NULL)
            {
                Node *next = node->next;
                RemoveNode(node);
                node = next;
            }
            m_array[i] = NULL;
        }
        m_count = 0;
        return true;
    }

    template <typename T>
    T HashHelper<T>::Get(std::string key, T defaultValue)
    {
        Node *find = NULL;
        if (GetNode(key, &find) == true)
            return find->value;

        return defaultValue;
    }

    template <typename T>
    T &HashHelper<T>::operator[](std::string key)
    {
        Node *find = NULL;
        if (GetNode(key, &find) == true)
            return find->value;

        throw "No this key in the hash table";
    }

    template <typename T>
    int HashHelper<T>::CalcCapacity(int capacity, int &cmpCapacity)
    {
        if (capacity < 100)
            capacity = 100;

        //哈希表大小 0.75 load factor.
        int minimumBucketCount = capacity * 4 / 3;

        // Bucket count must be power of 2.
        int ret = 1;
        while (ret <= minimumBucketCount)
            ret <<= 1;
        cmpCapacity = ret * 3 / 4;
        return ret;
    }

    template <typename T>
    int HashHelper<T>::CalcHash(std::string key)
    {
        // AlgorithmBKDR
        unsigned int temp = 0;
        unsigned int seed = 131; //31 131 1313 13131
        unsigned int value = 0;
        for (int i = 0; i < key.size(); i++)
        {
            temp = temp * seed + key[i];
        }
        temp = temp & 0x7FFFFFFF;

        // 第二层
        value = ~(temp << 9);
        value ^= (((unsigned int)value) >> 14);
        value += (value << 4);
        value ^= (((unsigned int)value) >> 10);
        return (int)value;
    }

    template <typename T>
    int HashHelper<T>::CalcSeatIndex(int hash)
    {
        return (hash) & (m_capacity - 1);
    }

    template <typename T>
    void HashHelper<T>::ExpandIfNecessary()
    {
        if (m_count > m_cmpCapacity)
        {
            int newCapacity = m_capacity << 1;
            int size = sizeof(Node *) * m_capacity;
            Node **newArray = (Node **)malloc(size);
            if (newArray != NULL)
            {
                memset(newArray, 0, size);
            }

            for (int i = 0; i < m_capacity; i++)
            {
                Node *tmp = m_array[i];
                if (tmp == NULL)
                    continue;

                int newIndex = CalcSeatIndex(tmp->hash);
            }
        }
    }

    template <typename T>
    bool HashHelper<T>::CreatNode(int hash, std::string key, T value, Node **node)
    {
        *node = new Node();
        if (node == NULL)
            return false;

        (*node)->hash = hash;
        (*node)->key = key;
        (*node)->value = value;
        (*node)->next = NULL;
        return true;
    }

    template <typename T>
    bool HashHelper<T>::GetNode(std::string key, Node **node)
    {
        int hash = CalcHash(key);
        int index = CalcSeatIndex(hash);

        Node *find = m_array[index];
        while (find != NULL)
        {
            if (find->key != key)
            {
                find = find->next;
                continue;
            }

            *node = find;
            return true;
        }
        return false;
    }

    template <typename T>
    void HashHelper<T>::RemoveNode(Node *node)
    {
        if (node != NULL)
        {
            delete node;
            node = NULL;
        }
    }
} // namespace aigc
