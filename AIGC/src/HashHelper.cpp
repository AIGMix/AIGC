#include "HashHelper.h"

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
        free(m_array);
        m_array = NULL;
    }
}

template <typename TVALUE>
bool HashHelper<TVALUE>::Add(std::string key, TVALUE value)
{
    ExpandIfNecessary();
    int hash = CalcHash(key);
    int index = CalcSeatIndex(hash);

    Node *find = m_array[index];
    while(find != NULL)
    {
        if (find->key == key)
            return false;
        if (find->next == NULL)
            break;
        find = find->next;
    }

    Node* node = CreatNode(hash, key, value);
    if (node == NULL)
        return false;
    
    if (find != NULL)
        find->next = node;
    else
        m_array[index] = node; 
    
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
        return true;
    }
    return true;
}

template <typename TVALUE>
TVALUE& HashHelper<TVALUE>::Get(std::string key)
{
    int hash = CalcHash(key);
    int index = CalcSeatIndex(hash);

    Node *find = m_array[index];
    if (find == NULL)
        return true;

    while (find != NULL)
    {
        if (find->key != key)
        {
            find = find->next;
            continue;
        }

        return find->value;
    }
    return NULL;
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
    for (int i = 0; i < key.size(); i++)
    {
        temp = temp * seed + key[i];
    }
    temp = temp & 0x7FFFFFFF;

    // 第二层
    unsigned int hash = ~(temp << 9);
    hash ^= (((unsigned int)hash) >> 14);
    hash += (hash << 4);
    hash ^= (((unsigned int)hash) >> 10);
    return (int)hash;
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
        memset(newArray, 0, size);

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
HashHelper<TVALUE>::Node* HashHelper<TVALUE>::CreatNode(int hash, std::string key, TVALUE value)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL)
        return NULL;

    node->hash = hash;
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

template <typename TVALUE>
void HashHelper<TVALUE>::RemoveNode(Node *node)
{
    if (node != NULL) 
    {
        free(node);
        node = NULL;
    }
}



}