#include "HashHelper.h"
#include <string.h>

namespace aigc
{

#define MIN_CAPACITY 100

template<typename TVALUE>
HashHelper<TVALUE>::HashHelper(int capacity)
{
    m_count = 0;
    m_capacity = CalcCapacity(capacity, m_cmpCapacity);

    int size = sizeof(Node*) * m_capacity;
    m_array = (Node **)malloc(size);
    if (m_array == NULL)
    {
        m_capacity = 0;
        m_cmpCapacity = 0;
    }

    memset(m_array, 0, size);
}

template <typename TVALUE>
HashHelper<TVALUE>::~HashHelper()
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

template <typename TVALUE>
bool HashHelper<TVALUE>::Add(std::string key, TVALUE value)
{
    //查找节点，如果存在则覆盖值
    Node* find = NULL;
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
    while(find != NULL)
    {
        if (find->key == key)
            return false;
        if (find->next == NULL)
            break;
        find = find->next;
    }

    //新建节点
    Node* node = NULL;
    if (CreatNode(hash, key, value, &node) == false)
        return false;
    
    if (find != NULL)
        find->next = node;
    else
        m_array[index] = node; 
    
    m_count++;
    return true;
}

template <typename TVALUE>
bool HashHelper<TVALUE>::Remove(std::string key)
{
    int hash = CalcHash(key);
    int index = CalcSeatIndex(hash);

    Node *find = m_array[index];
    if (find == NULL)
        return true;
    
    Node* pre = find;
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

template <typename TVALUE>
bool HashHelper<TVALUE>::RemoveAll()
{
    for (int i = 0; i < m_capacity; i++)
    {
        Node* node = m_array[i];
        while (node != NULL)
        {
            Node* next = node->next;
            RemoveNode(node);
            node = next;
        }
        m_array[i] = NULL;
    }
    m_count = 0;
    return true;
}


template <typename TVALUE>
TVALUE HashHelper<TVALUE>::Get(std::string key, TVALUE defaultValue)
{
    Node* find = NULL;
    if (GetNode(key, &find) == true)
        return find->value;

    return defaultValue;
}

template <typename TVALUE>
TVALUE& HashHelper<TVALUE>::operator[](std::string key)
{
    Node* find = NULL;
    if (GetNode(key, &find) == true)
        return find->value;

    throw "No this key in the hash table";
}

template<typename TVALUE>
int HashHelper<TVALUE>::CalcCapacity(int capacity, int& cmpCapacity)
{
    if (capacity < MIN_CAPACITY)
        capacity = MIN_CAPACITY;

    //哈希表大小 0.75 load factor.
    int minimumBucketCount = capacity * 4 / 3;

    // Bucket count must be power of 2.
    int ret = 1;
    while (ret <= minimumBucketCount)
        ret <<= 1;
    cmpCapacity = ret * 3 / 4;
    return ret;
}

template <typename TVALUE>
int HashHelper<TVALUE>::CalcHash(std::string key)
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

template <typename TVALUE>
int HashHelper<TVALUE>::CalcSeatIndex(int hash)
{
    return (hash) & (m_capacity - 1);
}

template <typename TVALUE>
void HashHelper<TVALUE>::ExpandIfNecessary()
{
    if (m_count > m_cmpCapacity)
    {
        int newCapacity = m_capacity << 1;
        int size = sizeof(Node *) * m_capacity;
        Node** newArray = (Node **)malloc(size);
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

template <typename TVALUE>
bool HashHelper<TVALUE>::CreatNode(int hash, std::string key, TVALUE value, Node **node)
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

template <typename TVALUE>
bool HashHelper<TVALUE>::GetNode(std::string key, Node** node)
{
    int hash = CalcHash(key);
    int index = CalcSeatIndex(hash);

    Node* find = m_array[index];
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

template <typename TVALUE>
void HashHelper<TVALUE>::RemoveNode(Node *node)
{
    if (node != NULL) 
    {
        delete node;
        node = NULL;
    }
}

}

// int main()
// {
//     aigc::HashHelper<int> hash(10);
    
//     hash.Add("my", 10);
//     hash.Add("test", 20);
//     hash.Remove("my");
//     int value1 = hash.Get("my", -1);
//     int value2 = hash.Get("test", -1);
//     int value3 = hash["test"];
//     int value4 = hash["my"];

//     return 0;
// }